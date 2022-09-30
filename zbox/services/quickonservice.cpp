#include "quickonservice.h"

#include "base/gparams.h"
#include "controller.h"
#include "utils/constutil.h"

#include <Shlobj.h>
#include <tlhelp32.h>
#include <psapi.h>

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

static bool CreateChildProcess(void);

QuickOnService::QuickOnService(Controller *controllor, Yaml2Stream *config, QString type)
    : Service(controllor, config, type)
    , m_bIsExist(false)
    , m_hScHandle(NULL)
    , m_hScHandleVbox(NULL)
{
    m_hScHandle = OpenSCManager(NULL, NULL, GENERIC_ALL);

    memset(g_szProgmaFiles, 0, sizeof(g_szProgmaFiles));
    SHGetFolderPathA(0, CSIDL_PROGRAM_FILES, 0, 0, g_szProgmaFiles);

    printf("%s\n", g_szProgmaFiles);
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
    printf("%s\n", g_szVirtualBoxHome);

    memset(g_szVBoxManager, 0, sizeof(g_szVBoxManager));
    sprintf(g_szVBoxManager, "%s\\%s", g_szProgmaFiles, VBOXMANAGER_NAME);

    VBoxManageFullPath();

    char buf[2000] = { 0 };
    IsOvaExist(buf);
}

QuickOnService::~QuickOnService()
{
    if (m_hScHandle)
        CloseServiceHandle(m_hScHandle);
}

void QuickOnService::PrepareCMD()
{
    SECURITY_ATTRIBUTES saAttr;

    printf("\n->Start of parent execution.\n");

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
        ExecCmd(read_buffer, "setx /m \"VBOX_USER_HOME\" \"%s\"\r\n", g_szVirtualBoxHome);
        ExecCmd(read_buffer, "set \"VBOX_USER_HOME\" \"%s\"\r\n", g_szVirtualBoxHome);

        return;
    } while (0);

    if (g_hChildStd_OUT_Rd) CloseHandle(g_hChildStd_OUT_Rd);
    if (g_hChildStd_OUT_Wr) CloseHandle(g_hChildStd_OUT_Wr);
    if (g_hChildStd_IN_Rd) CloseHandle(g_hChildStd_IN_Rd);
    if (g_hChildStd_IN_Wr) CloseHandle(g_hChildStd_IN_Wr);

    g_hChildStd_OUT_Rd = g_hChildStd_OUT_Wr = NULL;
    g_hChildStd_IN_Rd = g_hChildStd_IN_Wr = NULL;
}

int QuickOnService::ExecCmd(char read_buffer[], const char* fmt, ...)
{
    char current_dir[MAX_PATH] = { 0 };
    GetCurrentDirectoryA(MAX_PATH, current_dir);
    strcat(current_dir, ">");

    char szCmd[4096] = { 0 };
    va_list ap;
    va_start(ap, fmt);
    vsprintf(szCmd, fmt, ap);
    va_end(ap);

    if (!strstr(szCmd, "\r\n"))
        strcat(szCmd, "\r\n");

    DWORD dwRead, dwWritten;
    printf("** START EXEC %s\n", szCmd);
    if (!WriteFile(g_hChildStd_IN_Wr, szCmd, strlen(szCmd), &dwWritten, NULL) || !dwWritten)
    {
        printf("write %s failed\n", szCmd);
        return -1;
    }

    DWORD dwTotalBytesAvail, dwTotalRead = 0;
    static DWORD dwTotal = 0;

    for (;;)
    {
        if (!PeekNamedPipe(g_hChildStd_OUT_Rd, NULL, 0, NULL, &dwTotalBytesAvail, NULL))
        {
            printf("PeekNamedPipe failed\n");
            break;
        }

        if (!dwTotalBytesAvail)
        {
            Sleep(100);
            continue;
        }

        if (!ReadFile(g_hChildStd_OUT_Rd, read_buffer + dwTotalRead, dwTotalBytesAvail, &dwRead, NULL))
        {
            printf("ReadFile Failed\n");
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
        {
            break;
        }
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
    printf("*** EXEC Result: %s\n\n", read_buffer);
    printf("====================== DONE ======================\n");

    return dwRead;
}

bool QuickOnService::installServiceImpl(SendProxy *proxy)
{
    char read_buffer[40960] = { 0 };
    if (IsInstalled() != 0)
    {
        // RunVirtualBoxMSI
        RunVirtualBoxMSI(read_buffer);
    }

    VBoxManageFullPath();

    if (IsOvaExist(read_buffer) <= 0 && ImportOVA(read_buffer) <= 0)
    {
        return false;
    }
        
    return Service::installServiceImpl(proxy);
}

QString QuickOnService::queryState()
{
    if (IsInstalled() != 0)
        return ConstUtil::U_SERVICE_UNKNOWN;

    char buf[1000] = { 0 };
    if (IsOvaExist(buf) <= 0)
        return ConstUtil::U_SERVICE_UNKNOWN;

    return Service::queryState();
}

bool QuickOnService::uninstallServiceImpl(SendProxy *proxy)
{
    Service::uninstallServiceImpl(proxy);
    char buf[3000] = { 0 };
    if (RemoveOVA(buf) <= 0)
        return false;

    return true;
}

bool QuickOnService::startServiceImpl(SendProxy *proxy)
{
    Service::startServiceImpl(proxy);

    char read_buffer[3000] = { 0 };
    if (ExecCmd(read_buffer, "\"%s\" startvm %s --type headless\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
        return false;

    return true;
}

bool QuickOnService::stopServiceImpl(SendProxy *proxy)
{
    Service::stopServiceImpl(proxy);

    char read_buffer[3000] = { 0 };
    if (ExecCmd(read_buffer, "\"%s\" controlvm %s poweroff\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
        return false;

    return true;
}

bool QuickOnService::killServiceImpl(SendProxy *proxy)
{
    return Service::killServiceImpl(proxy);
}

bool QuickOnService::restartServiceImpl(SendProxy *proxy)
{
    Service::restartServiceImpl(proxy);

    char read_buffer[3000] = { 0 };
    if (ExecCmd(read_buffer, "\"%s\" controlvm %s reset\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
        return false;

    return true;
}

bool QuickOnService::lazyInstallServiceImpl(SendProxy *proxy)
{
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

    if (st.dwCurrentState != SERVICE_RUNNING)
        return 3;

    return 0;
}

int QuickOnService::RunVirtualBoxMSI(char* buf)
{
    return ExecCmd(buf, "MsiExec.exe /i \"%s\" INSTALLDIR=\"%s\" VBOX_INSTALLDESKTOPSHORTCUT=0 VBOX_INSTALLQUICKLAUNCHSHORTCUT=0 /qn /promptrestart\r\n", g_szVirtualBoxMSI, g_szProgmaFiles);
}

int QuickOnService::IsOvaExist(char* buf)
{
    int ret = ExecCmd(buf, "\"%s\" list vms\r\n", g_szVBoxManager);
    if (ret <= 0) 
        return ret;

    printf("list vms done => %s\n", buf);
    char* p = strstr(buf, OVA_QUICKON_NAME);
    if (!p)
        return 0;
    
    return ret;
}

int QuickOnService::ImportOVA(char* buf)
{
    if (ExecCmd(buf, "\"%s\" import -n \"%s\"", g_szVBoxManager, g_sdzQuickOnOva) <= 0)
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
    printf("@@@@@@@@@@@@@ %s\n", suggested_name);
    char* p = strchr(suggested_name, '<');
    if (p) *p = '\0';
    strcat(suggested_name, OVA_QUICKON_NAME);
    printf(">>>>>>>>>>>> %s\n", suggested_name);
    // find sound card id:
    char ignore_sound_card[200] = { 0 };
    get_item_id(ignore_sound_card, "Sound card");
    // find usb
    char ignore_usb[200] = { 0 };
    get_item_id(ignore_usb, "USB controller");
    // find cd-rom 
    char ignore_cd_rom[200] = { 0 };
    get_item_id(ignore_cd_rom, "CD-ROM");
    return ExecCmd(buf, "\"%s\" import \"%s\" %s %s %s %s\r\n", g_szVBoxManager, g_sdzQuickOnOva, suggested_name, ignore_sound_card, ignore_usb, ignore_cd_rom);
}

int QuickOnService::RemoveOVA(char* buf)
{
    if (ExecCmd(buf, "\"%s\" unregistervm --delete %s\r\n", g_szVBoxManager, OVA_QUICKON_NAME) <= 0)
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

    printf(">>>%s<<<\n", g_szVBoxManager);
    CloseHandle(hProcess);
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
   QuickOnService::ExecCmd(buf, "chcp 437\r\n");

   return true;
}
