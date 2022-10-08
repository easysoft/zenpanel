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

#include <WinSock2.h>
#include <windows.h>

#include <versionhelpers.h>

#include <memory>
#include <string>

static int CheckEnv();

int main(int argc, char *argv[])
{
    WSAData data;
    WSAStartup(0x0202, &data);

    printf("QSslSocket= %s\n", QSslSocket::sslLibraryBuildVersionString().toStdString().c_str());

    QuickOnService::PrepareCMD();

    if (CheckEnv())
    {
        printf("check env failed\n");
        return 0;
    }

    qRegisterMetaType<std::shared_ptr<std::string>>("std::shared_ptr<std::string>");

    ZApplication a("zentao_zbox_app",argc, argv);

    if (a.isRunning()) {
        a.sendMessage("raise_window_active");
        return EXIT_SUCCESS;
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

static int CheckEnv()
{
    if (!IsWindows8OrGreater()) // < win10 || server2016
    {
        printf("invalid windows version 0x%08x", GetVersion());
        getchar();
        return -1;
    }
/*
    char buf[1024] = { 0 };
    QuickOnService::ExecCmd(buf, "wmic cpu get VirtualizationFirmwareEnabled\r\n");
    if (!strstr(buf, "TRUE"))
    {
        printf("VirtualizationFirmwareEnabled FALSE\n");
        getchar();
        return 4;
    }
*/
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
    printf("prossor(s) = %d\n", si.dwNumberOfProcessors);
    printf("total phys mem = %lld - limit %lld, disk free space = %lld\n", mem.ullTotalPhys, 500 * gb, bytes_free.QuadPart);

    if (!IsWindowsServer()) // win
    {
        if (si.dwNumberOfProcessors < 4)
        {
            printf("### dwNumberOfProcessors = %d\n", si.dwNumberOfProcessors);
            return 1;
        }

        if (mem.ullTotalPhys < (uint64_t)(8 * gb))
        {
            printf("### ullTotalPhys = %d\n", mem.ullTotalPhys);
            return 2;
        }

        if (bytes_free.QuadPart < (uint64_t)(50 * gb))
        {
            printf("### QuadPart = %d\n", bytes_free.QuadPart);
            return 3;
        }

        return 0;
    }

    if (si.dwNumberOfProcessors < 8)
    {
        printf("!!! dwNumberOfProcessors = %d\n", si.dwNumberOfProcessors);
        return 1;
    }

    if (mem.ullTotalPhys < (uint64_t)(32 * gb))
    {
        printf("!!! ullTotalPhys = %d\n", mem.ullTotalPhys);
        return 2;
    }

    if (bytes_free.QuadPart < (uint64_t)(500 * gb))
    {
        printf("!!! QuadPart = %d\n", bytes_free.QuadPart);
        return 3;
    }

    return 0;
}
