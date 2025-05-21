/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "mainwindow.h"

#include <QApplication>
#include <QVariant>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDebug>

#include "utils/envutil.h"
#include "controller.h"
#include "zapplication.h"

#include "spdlogwrapper.hpp"

#ifdef Q_OS_WIN
#include <direct.h>
#else
#include <sys/stat.h>
#include <sys/types.h>
#endif // Q_OS_WIN

int main(int argc, char *argv[])
{
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

    return a.exec();
}
