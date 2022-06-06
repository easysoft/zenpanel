#include "mainwindow.h"

#include <QApplication>
#include <QVariant>
#include <QFontDatabase>
#include <QTextCodec>
#include <QDebug>

#include "utils/envutil.h"
#include "controller.h"
#include "zapplication.h"

int main(int argc, char *argv[])
{
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
