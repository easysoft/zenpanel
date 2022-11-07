#include "quickonservice.h"

#include "base/gparams.h"
#include "controller.h"
#include "utils/constutil.h"
#include "hardware/hardwareinfo.h"
#include "mainwindow.h"
#include "utils/processutil.h"

#include "spdlogwrapper.hpp"

#include <QJsonObject>
#include <QJsonDocument>

#include <Shlobj.h>
#include <tlhelp32.h>
#include <psapi.h>
#include <Iphlpapi.h>

#include <random>
#include <vector>
#include <string>
#include <algorithm>

#include <stdio.h>
#include <stdlib.h>

static HANDLE g_hChildStd_IN_Rd = NULL;
static HANDLE g_hChildStd_IN_Wr = NULL;
static HANDLE g_hChildStd_OUT_Rd = NULL;
static HANDLE g_hChildStd_OUT_Wr = NULL;

static char g_szVirtualBoxMSI[] = "etc\\quickon\\VirtualBox-6.1.38-r153438.msi";
static char g_sdzQuickOnOva[] = "etc\\quickon\\QuickOn.ova";
static char g_szProgmaFiles[MAX_PATH] = { 0 };
static char g_szVirtualBoxHome[MAX_PATH] = { 0 };
static char g_szVBoxManager[MAX_PATH] = { 0 };

#define OVA_QUICKON_NAME    "QuickOn"
#define VBOXMANAGER_NAME    "VBoxManage.exe"
#define VBOXSDS_NAME        "VBoxSDS"

#define QUICK_ON_HOST       "https://api.qucheng.com"

static bool CreateChildProcess(void);
static std::string LocalIP();
static bool EnumRegPhysicalNetworkCard(std::vector<std::string>& physical_cards);
static bool EnumNetworkCardDesc(HKEY hKey, const char* sub_key, std::string& desc);
static bool EnumAdaptersInfo(std::string& local_ip, const std::vector<std::string>& physical_cards);

QuickOnService::QuickOnService(Controller *controllor, Yaml2Stream *config, QString type)
    : Service(controllor, config, type)
    , m_bIsExist(false)
    , m_hScHandle(NULL)
    , m_hScHandleVbox(NULL)
{    
    m_hScHandle = OpenSCManager(NULL, NULL, GENERIC_ALL);

    memset(g_szProgmaFiles, 0, sizeof(g_szProgmaFiles));
    SHGetFolderPathA(0, CSIDL_PROGRAM_FILES, 0, 0, g_szProgmaFiles);

    L_TRACE("g_szProgmaFiles: {0}", g_szProgmaFiles);
    sprintf(g_szProgmaFiles + strlen(g_szProgmaFiles), "\\%s", OVA_QUICKON_NAME);
    CreateDirectoryA(g_szProgmaFiles, NULL);
    strcat(g_szProgmaFiles, "\\data");
    CreateDirectoryA(g_szProgmaFiles, NULL);
    strcat(g_szProgmaFiles, "\\.VirtualBox");
    CreateDirectoryA(g_szProgmaFiles, NULL);

    SHGetFolderPathA(0, CSIDL_LOCAL_APPDATA, 0, 0, g_szVirtualBoxHome);
    sprintf(g_szVirtualBoxHome + strlen(g_szVirtualBoxHome), "\\%s", OVA_QUICKON_NAME);
    CreateDirectoryA(g_szVirtualBoxHome, NULL);
    strcat(g_szVirtualBoxHome, "\\.VirtualBox");
    CreateDirectoryA(g_szVirtualBoxHome, NULL);
    L_TRACE("g_szVirtualBoxHome: {0}", g_szVirtualBoxHome);

    memset(g_szVBoxManager, 0, sizeof(g_szVBoxManager));
    sprintf(g_szVBoxManager, "%s\\%s", g_szProgmaFiles, VBOXMANAGER_NAME);

    VBoxManageFullPath();
}

QuickOnService::~QuickOnService()
{
    if (m_hScHandle)
        CloseServiceHandle(m_hScHandle);
}

bool QuickOnService::QueryUrl(std::shared_ptr<std::string> domain, std::string& message)
{
    if (QueryUrlLocal(domain))
        return true;

    if (!QueryUrlNet(domain, message))
        return false;

    quickon_record record;
    return SignUrl(domain, message, record);
}

bool QuickOnService::SignUrl(std::shared_ptr<std::string> domain, std::string& message, quickon_record& record)
{
    QJsonObject obj;

    char sub[200] = { 0 };
    sscanf(domain->c_str(), "%[a-zA-Z0-9]", sub);
    const char* real_domain = domain->c_str() + strlen(sub) + 1;
    obj.insert("domain", real_domain);
    obj.insert("ip", LocalIP().c_str());
    obj.insert("secretKey", GetHardWareInfo().c_str());
    obj.insert("sub", sub);

    QJsonDocument doc;
    doc.setObject(obj);
    QString json = doc.toJson(QJsonDocument::Compact);

    std::shared_ptr<std::string> url_str(new std::string)
                            , json_str(new std::string(json.toStdString()))
                            , reply_str(new std::string);

    url_str->append(QUICK_ON_HOST).append("/api/qdnsv2/oss/record");

    L_TRACE("************ {0} @ {1} START EMIT", __FUNCTION__, __LINE__);
    emit HttpPostData(url_str, json_str, reply_str);
    L_TRACE("************ {0} @ {1} END EMIT", __FUNCTION__, __LINE__);
    L_TRACE("reply: = {0}", reply_str->c_str());

    // todo: ret
    return true;

    bool ret = !reply_str->empty();
    QJsonParseError e;
    doc = QJsonDocument::fromJson(reply_str->c_str(), &e);
    if (doc.isNull() || e.error != QJsonParseError::NoError)
		return false;

    if (!ret || doc["code"].toInt() != 200)
    {
        record.message = doc["message"].toString().toStdString();
        return false;
    }

    auto data = doc["data"];
    if (data.isUndefined() || !data.isNull())
        return false;

    auto data_obj = data.toObject();
    record.tls_cert_path = data_obj["tls_cert_path"].toString().toStdString();
    record.tls_key_path = data_obj["tls_key_path"].toString().toStdString();
    record.domain = data_obj["domain"].toString().toStdString();
    record.k8s_tls = data_obj["k8s-tls"].toString().toStdString();
    
    return ret;
}

void QuickOnService::QueryPortRnd(int& http_port, int& https_port)
{
    std::mt19937::result_type seed = std::random_device()()
        ^ std::chrono::duration_cast<std::chrono::seconds>(
            std::chrono::system_clock::now().time_since_epoch()
            ).count()
        ^ std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::high_resolution_clock::now().time_since_epoch()
            ).count()
        /* ^ more_external_random_stuff */ ;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> dis(8000, 9000);
    http_port = dis(gen);
    https_port = dis(gen);
    L_TRACE("------>>>> HTTP = {0}, HTTPS = {1}", http_port, https_port);
}

void QuickOnService::PrepareCMD()
{
    SECURITY_ATTRIBUTES saAttr;

    L_TRACE("->Start of parent execution");

    // Set the bInheritHandle flag so pipe handles are inherited.
    saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
    saAttr.bInheritHandle = TRUE;
    saAttr.lpSecurityDescriptor = NULL;

    do {
        // Create a pipe for the child process's STDOUT.
        if (!CreatePipe(&g_hChildStd_OUT_Rd, &g_hChildStd_OUT_Wr, &saAttr, 0) )
              break;

        // Ensure the read handle to the pipe for STDOUT is not inherited.
        if (!SetHandleInformation(g_hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0) )
              break;

        // Create a pipe for the child process's STDIN.
        if (!CreatePipe(&g_hChildStd_IN_Rd, &g_hChildStd_IN_Wr, &saAttr, 0))
            break;

        // Ensure the write handle to the pipe for STDIN is not inherited.
        if (!SetHandleInformation(g_hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0) )
            break;

        // Create the child process.
        if (!CreateChildProcess())
            break;

        // exec cmd
        char read_buffer[40960] = { 0 };
        ExecCmd(read_buffer, 0, 0, "setx /m \"VBOX_USER_HOME\" \"%s\"\r\n", g_szVirtualBoxHome);
        ExecCmd(read_buffer, 0, 0,"set \"VBOX_USER_HOME\" \"%s\"\r\n", g_szVirtualBoxHome);

        return;
    } while (0);

    if (g_hChildStd_OUT_Rd) CloseHandle(g_hChildStd_OUT_Rd);
    if (g_hChildStd_OUT_Wr) CloseHandle(g_hChildStd_OUT_Wr);
    if (g_hChildStd_IN_Rd) CloseHandle(g_hChildStd_IN_Rd);
    if (g_hChildStd_IN_Wr) CloseHandle(g_hChildStd_IN_Wr);

    g_hChildStd_OUT_Rd = g_hChildStd_OUT_Wr = NULL;
    g_hChildStd_IN_Rd = g_hChildStd_IN_Wr = NULL;
}

int QuickOnService::ExecCmd(char read_buffer[], Service *service, SendProxy *proxy, const char* fmt, ...)
{
    char current_dir[MAX_PATH] = { 0 };
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    strcat(current_dir, ">");

    char szCmd[4096] = { 0 };
    va_list ap;
    va_start(ap, fmt);
    vsprintf(szCmd, fmt, ap);
    va_end(ap);

    DWORD dwRead, dwWritten;
    L_TRACE("** START EXEC {0}", szCmd);
    if (proxy && service) proxy->toSend(service->getInfoMsg(szCmd));

    if (!strstr(szCmd, "\r\n"))
        strcat(szCmd, "\r\n");
    if (!WriteFile(g_hChildStd_IN_Wr, szCmd, strlen(szCmd), &dwWritten, NULL) || !dwWritten)
    {
        L_ERROR("write {0} failed", szCmd);
        return -1;
    }

    DWORD dwTotalBytesAvail, dwTotalRead = 0;
    static DWORD dwTotal = 0;

    for (;;)
    {
        if (!PeekNamedPipe(g_hChildStd_OUT_Rd, NULL, 0, NULL, &dwTotalBytesAvail, NULL))
        {
            L_TRACE("PeekNamedPipe failed");
            break;
        }

        if (!dwTotalBytesAvail)
        {
            Sleep(100);
            continue;
        }

        if (!ReadFile(g_hChildStd_OUT_Rd, read_buffer + dwTotalRead, dwTotalBytesAvail, &dwRead, NULL))
        {
            L_TRACE("ReadFile Failed");
            break;
        }

        dwTotalRead += dwRead;
        read_buffer[dwTotalRead] = 0;

        // is done?
        char* beg = read_buffer;
        bool done = false;
        for (;;)
        {
            beg = strstr(beg, current_dir);
            if (!beg) break;

            beg += strlen(current_dir);
            if (!beg[0])
            {
                done = true;
                break;
            }
        }

        if (done)
            break;
    }

    dwTotal += dwTotalRead;
    SetFilePointer(g_hChildStd_OUT_Rd, dwTotal, 0, FILE_CURRENT);

    // skip "dir>"
    for (;;)
    {
        char* d = strstr(read_buffer, current_dir);
        if (!d)
            break;

        memmove(d, d + strlen(current_dir), strlen(read_buffer) - strlen(current_dir) + 1);
    }

    char* d = strstr(read_buffer, szCmd);
    if (d)
    {
        d += strlen(szCmd);
        memmove(read_buffer, d, strlen(d) + 1);
    }
    L_TRACE("*** EXEC Result: {0}", read_buffer);
    L_TRACE("====================== DONE ======================");

    return dwRead;
}

void QuickOnService::SetupSignal()
{
    connect(this, SIGNAL(HttpPostData(std::shared_ptr<std::string>, std::shared_ptr<std::string>, std::shared_ptr<std::string>))
        , m_ctr->MainWin(), SLOT(OnHttpPostData(std::shared_ptr<std::string>, std::shared_ptr<std::string>, std::shared_ptr<std::string>))
        , Qt::BlockingQueuedConnection);
    connect(this, SIGNAL(NotifyQuickOnInfo(std::shared_ptr<std::string>, int, int))
        , m_ctr->MainWin(), SLOT(OnNotifyQuickOnInfo(std::shared_ptr<std::string>, int, int))
        , Qt::QueuedConnection);

    std::shared_ptr<std::string> domain(new std::string);
    int http_port = 0, https_port = 0;

    QueryUrlLocal(domain);
    QueryPortLocal(http_port, https_port);

    L_TRACE("======> {0} - {1}", http_port, https_port);
    emit NotifyQuickOnInfo(domain, http_port, https_port);
}

bool QuickOnService::installServiceImpl(SendProxy *proxy)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);

    char read_buffer[40960] = { 0 };
    if (IsInstalled() != 0)
        RunVirtualBoxMSI(read_buffer);

    VBoxManageFullPath();

    if (IsOvaExist(read_buffer) <= 0 && ImportOVA(read_buffer) <= 0)
    {
        L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
        return false;
    }
        
    return Service::installServiceImpl(proxy);
}
/*
QString QuickOnService::queryState()
{
    QString nowServiceName = serviceName();
    QString status = ProcessUtil::serviceState(nowServiceName);
    L_TRACE("{0} @ {1} - nowServiceName: {2}, status: {3}", __FUNCTION__, __LINE__, nowServiceName.toStdString().c_str(), status.toStdString().c_str());
    if (status == ConstUtil::U_SERVICE_UNKNOWN)
        return ConstUtil::U_SERVICE_UNKNOWN;

    if (IsInstalled() != 0)
        return ConstUtil::U_SERVICE_UNKNOWN;

    char buf[1000] = { 0 };
    if (IsOvaExist(buf) <= 0)
        return ConstUtil::U_SERVICE_UNKNOWN;

    return status;
}
*/
bool QuickOnService::uninstallServiceImpl(SendProxy *proxy)
{
    Service::uninstallServiceImpl(proxy);
    char buf[3000] = { 0 };
    if (RemoveOVA(buf) <= 0)
        return false;

    char local_file_name[MAX_PATH] = { 0 };
    sprintf(local_file_name, "%s\\init\\env", g_szVirtualBoxHome);
    FILE* fp = fopen(local_file_name, "w+");
    if (fp) fclose(fp);

    return true;
}

bool QuickOnService::startServiceImpl(SendProxy *proxy)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);
    char read_buffer[40960] = { 0 };
    if (IsInstalled() != 0)
        RunVirtualBoxMSI(read_buffer);

    VBoxManageFullPath();

    if (IsOvaExist(read_buffer) <= 0 && ImportOVA(read_buffer) <= 0)
    {
        L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
        return false;
    }

    std::shared_ptr<std::string> domain(new std::string);
    std::string message;
    if (!QueryUrl(domain, message))
    {
        L_ERROR("QueryUrl Failed: {0} @ {1}", __FUNCTION__, __LINE__);
        proxy->toSend(getErrorMsg(message.c_str()));
        return false;
    }
    L_TRACE("{0} @ {1} domain: {2}", __FUNCTION__, __LINE__, domain->c_str());

    int http_port = 0, https_port = 0;
    QueryPortLocal(http_port, https_port);
    if (!http_port || !https_port)
        QueryPortRnd(http_port, https_port);

    printf("======> %d - %d\n", http_port, https_port);
    
    SaveInitEnv(domain, http_port, https_port);

    emit NotifyQuickOnInfo(domain, 0, 0);

    // 启动虚拟机
    printf("$$$$$$$$$$ %s @ %d: startvm\n", __FUNCTION__, __LINE__);
    if (ExecCmd(read_buffer, this, proxy, "\"%s\" startvm %s --type headless\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
    {
        proxy->toSend(getErrorMsg(read_buffer));
        return false;
    }

    // 端口转发
    // 80
    printf("$$$$$$$$$$ %s @ %d: natpf1 80\n", __FUNCTION__, __LINE__);
    if (ExecCmd(read_buffer, this, proxy, "\"%s\" controlvm \"%s\" natpf1 \"http,tcp,,%d,,80\"\r\n", g_szVBoxManager, OVA_QUICKON_NAME, http_port) <= 0)
    {
        proxy->toSend(getErrorMsg(read_buffer));
        return false;
    }
    // 443
    printf("$$$$$$$$$$ %s @ %d: natpf1 443\n", __FUNCTION__, __LINE__);
    if (ExecCmd(read_buffer, this, proxy, "\"%s\" controlvm \"%s\" natpf1 \"https,tcp,,%d,,443\"\r\n", g_szVBoxManager, OVA_QUICKON_NAME, https_port) <= 0)
    {
        proxy->toSend(getErrorMsg(read_buffer));
        return false;
    }

    // 设置共享目录
    printf("$$$$$$$$$$ %s @ %d: sharedfolder\n", __FUNCTION__, __LINE__);
    if (ExecCmd(read_buffer, this, proxy, "\"%s\" sharedfolder add %s --name=env --hostpath=\"%s\\init\" --readonly --transient --automount --auto-mount-point=/mnt\r\n", g_szVBoxManager, OVA_QUICKON_NAME, g_szVirtualBoxHome) <= 0)
    {
        proxy->toSend(getErrorMsg(read_buffer));
        return false;
    }

    printf("$$$$$$$$$$ %s @ %d: startServiceImpl\n", __FUNCTION__, __LINE__);
    if (!Service::startServiceImpl(proxy))
        return false;

    emit NotifyQuickOnInfo(domain, http_port, https_port);

    printf("$$$$$$$$$$ %s @ %d: DONE\n", __FUNCTION__, __LINE__);

    return true;
}

bool QuickOnService::stopServiceImpl(SendProxy *proxy)
{
    printf("%s @ %d\n", __FUNCTION__, __LINE__);

    Service::stopServiceImpl(proxy);

    char read_buffer[3000] = { 0 };
    if (ExecCmd(read_buffer, this, proxy,"\"%s\" controlvm %s poweroff\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
    {
        proxy->toSend(getErrorMsg(read_buffer));
        return false;
    }

    return true;
}

bool QuickOnService::killServiceImpl(SendProxy *proxy)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);

    return Service::killServiceImpl(proxy);
}

bool QuickOnService::restartServiceImpl(SendProxy *proxy)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);

    Service::restartServiceImpl(proxy);

    char read_buffer[3000] = { 0 };
    if (ExecCmd(read_buffer, this, proxy,"\"%s\" controlvm %s reset\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
        return false;

    return true;
}

bool QuickOnService::lazyInstallServiceImpl(SendProxy *proxy)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);

    return installServiceImpl(proxy);
}

int QuickOnService::IsInstalled()
{
    if (!m_hScHandle)
        return -1;

    m_hScHandleVbox = OpenServiceA(m_hScHandle, VBOXSDS_NAME, SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP);
    if (!m_hScHandleVbox)
        return 1;

    SERVICE_STATUS st;
    if (!QueryServiceStatus(m_hScHandleVbox, &st))
    {
        CloseHandle(m_hScHandleVbox);
        m_hScHandleVbox = NULL;

        return 2;
    }

    CloseHandle(m_hScHandleVbox);
    m_hScHandleVbox = NULL;

    return 0;
}

int QuickOnService::RunVirtualBoxMSI(char* buf)
{
    return ExecCmd(buf, this, 0,"MsiExec.exe /i \"%s\" INSTALLDIR=\"%s\" VBOX_INSTALLDESKTOPSHORTCUT=0 VBOX_INSTALLQUICKLAUNCHSHORTCUT=0 /qn /promptrestart\r\n", g_szVirtualBoxMSI, g_szProgmaFiles);
}

int QuickOnService::IsOvaExist(char* buf)
{
    int ret = ExecCmd(buf, 0, 0,"\"%s\" list vms\r\n", g_szVBoxManager);
    if (ret <= 0) 
        return ret;

    char* p = strstr(buf, OVA_QUICKON_NAME);
    L_TRACE("#### {0}", p ? "HAS OVA" : "NO OVA");
    if (!p)
        return 0;
    
    return ret;
}

int QuickOnService::ImportOVA(char* buf)
{
    if (ExecCmd(buf, 0, 0,"\"%s\" import -n \"%s\"", g_szVBoxManager, g_sdzQuickOnOva) <= 0)
        return 0;
        
    auto get_item_id = [buf](char* dst, const char* key)
    {
        const char* p = strstr(buf, key);
        if (!p) return -1;

        p = strchr(p, '(');
        if (!p) return -2;
        p = strchr(p, '\"');
        if (!p) return -3;

        p++;
        sscanf(p, "%[^\"]", dst);
        return 0;
    };
    // find Suggested VM name
    char suggested_name[200] = { 0 };
    get_item_id(suggested_name, "Suggested VM name");
    char* p = strchr(suggested_name, '<');
    if (p) *p = '\0';
    strcat(suggested_name, OVA_QUICKON_NAME);
    // find sound card id:
    char ignore_sound_card[200] = { 0 };
    get_item_id(ignore_sound_card, "Sound card");
    // find usb
    char ignore_usb[200] = { 0 };
    get_item_id(ignore_usb, "USB controller");
    // find cd-rom 
    char ignore_cd_rom[200] = { 0 };
    get_item_id(ignore_cd_rom, "CD-ROM");
    return ExecCmd(buf, 0, 0,"\"%s\" import \"%s\" %s %s %s %s\r\n", g_szVBoxManager, g_sdzQuickOnOva, suggested_name, ignore_sound_card, ignore_usb, ignore_cd_rom);
}

int QuickOnService::RemoveOVA(char* buf)
{
    if (ExecCmd(buf, 0, 0,"\"%s\" unregistervm --delete %s\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
        return false;

    return true;
}

void QuickOnService::VBoxManageFullPath()
{
    UINT pid = 0;
    PROCESSENTRY32 pe32;
    memset(&pe32, 0, sizeof(pe32));
    pe32.dwSize = sizeof(pe32);

    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE)
        return;

    char szExeFile[MAX_PATH] = { 0 };
    if (!Process32First(hProcessSnap, &pe32))
    {
        CloseHandle(hProcessSnap);
        return;
    }

    do
    {
        memset(szExeFile, 0, sizeof(szExeFile));
        WideCharToMultiByte(CP_ACP, 0, pe32.szExeFile, wcslen(pe32.szExeFile), szExeFile, MAX_PATH, 0, 0);
        if (strstr(szExeFile, VBOXSDS_NAME))
        {
            pid = pe32.th32ProcessID;
            break;
        }
    } while (Process32Next(hProcessSnap, &pe32));

    CloseHandle(hProcessSnap);

    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pid);
    if(!hProcess)
        return;

    memset(g_szVBoxManager, 0, sizeof(g_szVBoxManager));
    GetModuleFileNameExA(hProcess, NULL, g_szVBoxManager, MAX_PATH);
    char* p = strrchr(g_szVBoxManager, '\\');
    if (p)
    {
        p[1] = 0;
        strcat(g_szVBoxManager, VBOXMANAGER_NAME);
    }

    CloseHandle(hProcess);
}

bool QuickOnService::QueryUrlLocal(std::shared_ptr<std::string> domain)
{
    /*
        QUICKON_DOMAIN=demo.haogs.cn
        QUICKON_HTTP_PORT=8080
        QUICKON_HTTPS_PORT=8443
    */
    char local_file_name[MAX_PATH] = { 0 };
    sprintf(local_file_name, "%s\\init\\env", g_szVirtualBoxHome);
    FILE* fp = fopen(local_file_name, "rt");
    if (!fp)
    {
        L_ERROR("{0} not exist", local_file_name);
        return false;
    }

    char line[1000] = { 0 };
    while (fgets(line, sizeof(line) / sizeof(line[0]), fp)) 
    {
        const char* p = strstr(line, "QUICKON_DOMAIN");
        if (p)
        {
            p = strrchr(line, '=');
            if (!p)
                break;
            p++;
            *domain = p;
            while (*domain->rbegin() == '\r' || *domain->rbegin() == '\n')
                domain->pop_back();
            break;
        }
    }
    fclose(fp);

    return !domain->empty();
}

bool QuickOnService::QueryUrlNet(std::shared_ptr<std::string> domain, std::string& message)
{
    QJsonObject obj;
    obj.insert("domain", "");
    obj.insert("secretKey", GetHardWareInfo().c_str());
    obj.insert("sub", "");

    QJsonDocument doc;
    doc.setObject(obj);
    QString json = doc.toJson(QJsonDocument::Compact);

    std::shared_ptr<std::string> url_str(new std::string)
                            , json_str(new std::string(json.toStdString()))
                            , reply_str(new std::string);

    url_str->append(QUICK_ON_HOST).append("/api/qdnsv2/oss/custom");

    L_TRACE("************ {0} @ {1} START EMIT", __FUNCTION__, __LINE__);
    emit HttpPostData(url_str, json_str, reply_str);
    bool ret = !reply_str->empty();
    L_TRACE("************ {0} @ {1} END EMIT", __FUNCTION__, __LINE__);
    L_TRACE("reply = {0}", reply_str->c_str());
    QJsonParseError e;
    doc = QJsonDocument::fromJson(reply_str->c_str(), &e);
    if (doc.isNull() || e.error != QJsonParseError::NoError)
		return false;

    if (!ret || doc["code"].toInt() != 200)
    {
        message = doc["message"].toString().toStdString();
        L_ERROR("{0} @ {1}", __FUNCTION__, __LINE__);
        return false;
    }

    auto data = doc["data"];
    if (data.isUndefined() || data.isNull())
    {
        L_ERROR("{0} @ {1}", __FUNCTION__, __LINE__);
        return false;
    }

    auto data_obj = data.toObject();
    *domain = data_obj["domain"].toString().toStdString();
    
    return !domain->empty();
}

void QuickOnService::QueryPortLocal(int& http_port, int& https_port)
{
    /*
        QUICKON_DOMAIN=demo.haogs.cn
        QUICKON_HTTP_PORT=8080
        QUICKON_HTTPS_PORT=8443
    */
    char local_file_name[MAX_PATH] = { 0 };
    sprintf(local_file_name, "%s\\init\\env", g_szVirtualBoxHome);
    FILE* fp = fopen(local_file_name, "rt");
    if (!fp)
        return;

    http_port = https_port = 0;
    char line[1000] = { 0 };
    while ((!http_port || !https_port) && fgets(line, sizeof(line) / sizeof(line[0]), fp)) 
    {
        const char* p;
        p = strstr(line, "QUICKON_HTTP_PORT");
        if (p)
        {
            p = strrchr(line, '=');
            if (p) http_port = atoi(p + 1);

            continue;
        }

        p = strstr(line, "QUICKON_HTTPS_PORT");
        if (p)
        {
            p = strrchr(line, '=');
            if (p) https_port = atoi(p + 1);

            continue;
        }
    }
    fclose(fp);
}

void QuickOnService::SaveInitEnv(std::shared_ptr<std::string> domain, int http_port, int https_port)
{
    char local_file_name[MAX_PATH] = { 0 };
    sprintf(local_file_name, "%s\\init", g_szVirtualBoxHome);
    CreateDirectoryA(local_file_name, NULL);
    strcat(local_file_name, "\\env");
    FILE* fp = fopen(local_file_name, "w+t");
    if (!fp)
    {
        L_ERROR("Can not open file {0}", local_file_name);
        return;
    }

    fprintf(fp, "QUICKON_DOMAIN=%s\n", domain->c_str());
    fprintf(fp, "QUICKON_HTTP_PORT=%d\n", http_port);
    fprintf(fp, "QUICKON_HTTPS_PORT=%d\n", https_port);
    fflush(fp);
    fclose(fp);
}

static bool CreateChildProcess()
// Create a child process that uses the previously created pipes for STDIN and STDOUT.
{
   PROCESS_INFORMATION piProcInfo;
   STARTUPINFOA siStartInfo;
   BOOL bSuccess = FALSE;

// Set up members of the PROCESS_INFORMATION structure.

   ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION) );

// Set up members of the STARTUPINFO structure.
// This structure specifies the STDIN and STDOUT handles for redirection.

   ZeroMemory(&siStartInfo, sizeof(STARTUPINFO) );
   siStartInfo.cb = sizeof(siStartInfo);
   GetStartupInfoA(&siStartInfo);
   siStartInfo.hStdError = g_hChildStd_OUT_Wr;
   siStartInfo.hStdOutput = g_hChildStd_OUT_Wr;
   siStartInfo.hStdInput = g_hChildStd_IN_Rd;
   siStartInfo.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
   siStartInfo.wShowWindow = SW_HIDE;

// Create the child process.

   bSuccess = CreateProcessA(NULL,
      "CMD.exe",     // command line
      NULL,          // process security attributes
      NULL,          // primary thread security attributes
      TRUE,          // handles are inherited
      CREATE_NO_WINDOW,             // creation flags
      NULL,          // use parent's environment
      NULL,          // use parent's current directory
      &siStartInfo,  // STARTUPINFO pointer
      &piProcInfo);  // receives PROCESS_INFORMATION

   // If an error occurs, exit the application.
   if (!bSuccess )
       return false;

   // Close handles to the child process and its primary thread.
   // Some applications might keep these handles to monitor the status
   // of the child process, for example.
   CloseHandle(piProcInfo.hProcess);
   CloseHandle(piProcInfo.hThread);

   // Close handles to the stdin and stdout pipes no longer needed by the child process.
   // If they are not explicitly closed, there is no way to recognize that the child process has ended.
   CloseHandle(g_hChildStd_OUT_Wr);
   CloseHandle(g_hChildStd_IN_Rd);

   g_hChildStd_OUT_Wr = g_hChildStd_IN_Rd = NULL;

   char buf[1024] = { 0 };
   QuickOnService::ExecCmd(buf, 0, 0, "chcp 437\r\n");

   return true;
}

static std::string LocalIP()
{
    std::vector<std::string> physical_cards;
    if (!EnumRegPhysicalNetworkCard(physical_cards))
        return "";

    std::string local_ip;
    if (EnumAdaptersInfo(local_ip, physical_cards))
        return local_ip;

    return "";
}


static bool EnumRegPhysicalNetworkCard(std::vector<std::string>& physical_cards)
{
    HKEY hKey = 0;
    if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Control\\Class\\{4d36e972-e325-11ce-bfc1-08002be10318}", 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hKey) != ERROR_SUCCESS)
        return false;

    char str[1000] = { 0 };
    DWORD len = sizeof(str);
    DWORD index = 0;

    std::string desc;

    for (;;) {
        LSTATUS st = RegEnumKeyExA(hKey, index, str, &len, 0, 0, 0, 0);
        if (st != ERROR_SUCCESS && st != ERROR_MORE_DATA)
            break;

        str[len] = 0;

        desc.clear();
        if (EnumNetworkCardDesc(hKey, str, desc))
            physical_cards.push_back(desc);

        len = sizeof(str);
        index++;
    }

    RegCloseKey(hKey);

    return !physical_cards.empty();
}

static bool EnumNetworkCardDesc(HKEY hKey, const char* sub_key, std::string& desc)
{
    HKEY hSubKey;
    if (RegOpenKeyExA(hKey, sub_key, 0, KEY_QUERY_VALUE | KEY_ENUMERATE_SUB_KEYS, &hSubKey) != ERROR_SUCCESS)
        return false;

    do
    {
        DWORD Characteristics;
        DWORD size = sizeof(Characteristics);
        DWORD type = REG_DWORD;
        if (RegQueryValueExA(hSubKey, "Characteristics", 0, &type, (LPBYTE)&Characteristics, &size) != ERROR_SUCCESS)
            break;

        if (Characteristics != 0x84)
            break;

        char DriverDesc[1000] = { 0 };
        size = sizeof(DriverDesc);
        type = REG_SZ;
        if (RegQueryValueExA(hSubKey, "DriverDesc", 0, &type, (LPBYTE)DriverDesc, &size) != ERROR_SUCCESS)
            break;

        desc = DriverDesc;
    } while (0);

    RegCloseKey(hSubKey);

    return !desc.empty();
}

static bool EnumAdaptersInfo(std::string& local_ip, const std::vector<std::string>& physical_cards)
{
    //PIP_ADAPTER_INFO结构体指针存储本机网卡信息
    PIP_ADAPTER_INFO pIpAdapterInfo = new IP_ADAPTER_INFO();
    //得到结构体大小,用于GetAdaptersInfo参数
    unsigned long stSize = sizeof(IP_ADAPTER_INFO);
    //调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量;其中stSize参数既是一个输入量也是一个输出量
    int nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    if (ERROR_BUFFER_OVERFLOW == nRel)
    {
        //如果函数返回的是ERROR_BUFFER_OVERFLOW
        //则说明GetAdaptersInfo参数传递的内存空间不够,同时其传出stSize,表示需要的空间大小
        //这也是说明为什么stSize既是一个输入量也是一个输出量
        //释放原来的内存空间
        delete pIpAdapterInfo;
        //重新申请内存空间用来存储所有网卡信息
        pIpAdapterInfo = (PIP_ADAPTER_INFO)new BYTE[stSize];
        //再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
        nRel = GetAdaptersInfo(pIpAdapterInfo, &stSize);
    }

    if (ERROR_SUCCESS != nRel)
        return false;

    do
    {
        //输出网卡信息
        //可能有多网卡,因此通过循环去判断
        for (PIP_ADAPTER_INFO p = pIpAdapterInfo; p; p = p->Next)
        {
            if (std::find(physical_cards.begin(), physical_cards.end(), p->Description) == physical_cards.end())
                continue;

            L_TRACE("name: {0}, desc: {1}", p->AdapterName, p->Description);
            //可能网卡有多IP,因此通过循环去判断
            IP_ADDR_STRING* pIpAddrString = &(p->IpAddressList);
            do
            {
                L_TRACE("### addr: {0}", pIpAddrString->IpAddress.String);

                if (strlen(pIpAddrString->IpAddress.String) && strcmp(pIpAddrString->IpAddress.String, "0.0.0.0"))
                {
                    local_ip = pIpAddrString->IpAddress.String;
                    break;
                }

                pIpAddrString = pIpAddrString->Next;
            } while (pIpAddrString);
        }
    } while (0);

    //释放内存空间
    if (pIpAdapterInfo)
    {
        delete pIpAdapterInfo;
    }

    return !local_ip.empty();
}