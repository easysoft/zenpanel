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

#include <QMessageBox>

#include <memory>
#include <string>

static int CheckEnv();

int main(int argc, char *argv[])
{
    WSAData data;
    WSAStartup(0x0202, &data);

    printf("QSslSocket= %s\n", QSslSocket::sslLibraryBuildVersionString().toStdString().c_str());

    qRegisterMetaType<std::shared_ptr<std::string>>("std::shared_ptr<std::string>");

    ZApplication a("zentao_zbox_app",argc, argv);

    if (a.isRunning()) {
        a.sendMessage("raise_window_active");
        return EXIT_SUCCESS;
    }

    QuickOnService::PrepareCMD();

    if (CheckEnv())
    {
        printf("check env failed\n");
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

static int CheckEnv()
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
    if (!IsWindowsServer()) // win
    {
        if (si.dwNumberOfProcessors < 4)
        {
            QMessageBox::information(nullptr, u8"提示", u8"需要4核心以上处理器");
            return 1;
        }

        if (mem.ullTotalPhys < (uint64_t)(8 * gb))
        {
            QMessageBox::information(nullptr, u8"提示", u8"需要8GB以上内存");
            return 2;
        }

        if (bytes_free.QuadPart < (uint64_t)(50 * gb))
        {
            QMessageBox::information(nullptr, u8"提示", u8"需要50GB以上硬盘");
            return 3;
        }

        return 0;
    }

    if (si.dwNumberOfProcessors < 8)
    {
        QMessageBox::information(nullptr, u8"提示", u8"需要8核心以上处理器");
        return 1;
    }

    if (mem.ullTotalPhys < (uint64_t)(32 * gb))
    {
        QMessageBox::information(nullptr, u8"提示", u8"需要32GB以上内存");
        return 2;
    }

    if (bytes_free.QuadPart < (uint64_t)(500 * gb))
    {
        QMessageBox::information(nullptr, u8"提示", u8"需要500GB以上硬盘");
        return 3;
    }

    return 0;
}
