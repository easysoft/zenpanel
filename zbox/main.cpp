#include "mainwindow.h"

#include <QApplication>
#include <QVariant>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDebug>

#include "utils/envutil.h"
#include "services/quickonservice.h"
#include "controller.h"
#include "zapplication.h"

#include "spdlogwrapper.hpp"

#include <WinSock2.h>
#include <windows.h>

#include <versionhelpers.h>

#include <QMessageBox>
#include <QCommandLineParser>

#include <memory>
#include <string>

#ifdef Q_OS_WIN
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif // Q_OS_WIN
static int CheckEnv(QCommandLineParser& parser);

int main(int argc, char *argv[])
{
    WSAData data;
    WSAStartup(0x0202, &data);

    printf("QSslSocket= %s, supportsSsl = %s\n", QSslSocket::sslLibraryBuildVersionString().toStdString().c_str(), QSslSocket::supportsSsl() ? "true" : "false");

    qRegisterMetaType<std::shared_ptr<std::string>>("std::shared_ptr<std::string>");

#ifdef USE_SPDLOG_
#ifdef Q_OS_WIN
    mkdir("logs");
#else
    mkdir("logs", S_IRWXU);
#endif // Q_OS_WIN
    spdlog::spdlog_init("zenshot", "logs/log.log", 23, 57, 0, 0);
    L_TRACE("start");
#endif // USE_SPDLOG_

    ZApplication a("zentao_zbox_app",argc, argv);

    if (a.isRunning()) {
        a.sendMessage("raise_window_active");
        return EXIT_SUCCESS;
    }

    QuickOnService::PrepareCMD();

    QCommandLineParser parser;
    parser.setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);
    QCommandLineOption win_proc_num("p", "processor", "winprocessor", "4");
    parser.addOption(win_proc_num);
    QCommandLineOption server_proc_num("P", "processor", "serverprocessor", "8");
    parser.addOption(server_proc_num);
    QCommandLineOption win_mem_gb("m", "memory", "winmemory", "8");
    parser.addOption(win_mem_gb);
    QCommandLineOption server_mem_gb("M", "memory", "servermemory", "32");
    parser.addOption(server_mem_gb);
    QCommandLineOption win_disk_gb("d", "disk", "windisk", "50");
    parser.addOption(win_disk_gb);
    QCommandLineOption server_disk_gb("D", "disk", "serverdisk", "500");
    parser.addOption(server_disk_gb);
    parser.process(a);
    if (CheckEnv(parser))
    {
        L_ERROR("check env failed\n");
        return 0;
    }

    QString rootPath = QCoreApplication::applicationDirPath() + "/";
    EnvUtil::setRootPath(rootPath);

    QFont font;
    font.setPixelSize(14);
    font.setFamily("微软雅黑");
    a.setFont(font);

    int fontId = QFontDatabase::addApplicationFont(EnvUtil::getPath("zbox/res/iconfont.ttf"));
    QString fontNmae = QFontDatabase::applicationFontFamilies(fontId).at(0);


    Controller controller;

    QObject::connect(&a, SIGNAL(messageReceived(const QString&)), &controller, SLOT(toActiveWindow(const QString&)));

    a.exec();

    WSACleanup();

    return 0;
}

static int CheckEnv(QCommandLineParser& parser)
{
    if (!IsWindows8OrGreater()) // < win10 || server2016
    {
        QMessageBox::information(nullptr, u8"提示", u8"系统版本过低");
        return -1;
    }

    // VT-D
    char buf[1024] = { 0 };
    QuickOnService::ExecCmd(buf, nullptr, nullptr, "wmic cpu get VirtualizationFirmwareEnabled\r\n");
    if (!strstr(buf, "TRUE"))
    {
        QMessageBox::information(nullptr, u8"提示", u8"请先开启系统虚拟化");
        return 4;
    }

    SYSTEM_INFO si;
    GetSystemInfo(&si);

    MEMORYSTATUSEX mem;
    mem.dwLength = sizeof(mem);
    GlobalMemoryStatusEx(&mem);

    char sys_dir[MAX_PATH] = { 0 };
    GetSystemDirectoryA(sys_dir, MAX_PATH);
    sys_dir[3] = 0;

    ULARGE_INTEGER bytes_available;
    ULARGE_INTEGER bytes_total;
    ULARGE_INTEGER bytes_free;
    GetDiskFreeSpaceExA(sys_dir, &bytes_available, &bytes_total, &bytes_free);

    uint64_t gb = 1024 * 1024 * 1024;

    uint64_t processor, mem_limit, disk_limit;
    QString tip;
    if (!IsWindowsServer()) // win
    {
        processor = parser.value("p").toULongLong();
        L_TRACE(">>>> processor = {0} -- {1}", processor, parser.value("P").toULongLong());
        if (si.dwNumberOfProcessors < processor)
        {
            tip = u8"需要" + QString(std::to_string(processor).c_str()) + u8"核心以上处理器";
            QMessageBox::information(nullptr, u8"提示", tip);
            return 1;
        }

        mem_limit = parser.value("m").toULongLong();
        L_TRACE(">>>> memory = {0} -- {1}", mem_limit, parser.value("M").toULongLong());
        if (mem.ullTotalPhys < (uint64_t)(mem_limit * gb))
        {
            tip = u8"需要" + QString(std::to_string(mem_limit).c_str()) + u8"GB以上内存";
            QMessageBox::information(nullptr, u8"提示", tip);
            return 2;
        }

        disk_limit = parser.value("d").toULongLong();
        L_TRACE(">>>> disk = {0} -- {1}", disk_limit, parser.value("D").toULongLong());
        if (bytes_free.QuadPart < (uint64_t)(disk_limit * gb))
        {
            tip = u8"需要" + QString(std::to_string(disk_limit).c_str()) + u8"GB以上硬盘";
            QMessageBox::information(nullptr, u8"提示", tip);
            return 3;
        }

        return 0;
    }

    processor = parser.value("P").toULongLong();
    if (si.dwNumberOfProcessors < processor)
    {
        tip = u8"需要" + QString(std::to_string(processor).c_str()) + u8"核心以上处理器";
        QMessageBox::information(nullptr, u8"提示", tip);
        return 1;
    }

    mem_limit = parser.value("M").toULongLong();
    if (mem.ullTotalPhys < (uint64_t)(mem_limit * gb))
    {
        tip = u8"需要" + QString(std::to_string(mem_limit).c_str()) + u8"GB以上内存";
        QMessageBox::information(nullptr, u8"提示", tip);
        return 2;
    }

    disk_limit = parser.value("D").toULongLong();
    if (bytes_free.QuadPart < (uint64_t)(disk_limit * gb))
    {
        tip = u8"需要" + QString(std::to_string(disk_limit).c_str()) + u8"GB以上硬盘";
        QMessageBox::information(nullptr, u8"提示", tip);
        return 3;
    }

    return 0;
}
