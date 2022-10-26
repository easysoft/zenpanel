QT       += core gui

RC_FILE += uac.rc


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    base/gbus.cpp \
    base/glang.cpp \
    base/gparams.cpp \
    base/gscale.cpp \
    base/runner.cpp \
    base/sendproxy.cpp \
    base/service.cpp \
    base/servicebase.cpp \
    configs/yaml2stream.cpp \
    configs/yamlstream.cpp \
    base/zoffsetmenu.cpp \
    base/zpopupbutton.cpp \
    base/zpopupmenu.cpp \
    base/zwidget.cpp \
    controller.cpp \
    controls/apachesetting.cpp \
    controls/askconfirm.cpp \
    controls/askpassword.cpp \
    controls/closedialog.cpp \
    controls/globalcontrol.cpp \
    controls/infomessage.cpp \
    controls/logcontainer.cpp \
    controls/modifyaccount.cpp \
    controls/modifyport.cpp \
    controls/servicecontainer.cpp \
    controls/servicecontrol.cpp \
    controls/vcinstallask.cpp \
    runners/appinstaller.cpp \
    runners/backupruner.cpp \
    runners/envrunner.cpp \
    runners/modifyrunner.cpp \
    main.cpp \
    mainwindow.cpp \
    runners/serviceinstaller.cpp \
    servicehelper.cpp \
    services/apacheservice.cpp \
    services/groupservice.cpp \
    services/mysqlservice.cpp \
    services/productservice.cpp \
    services/quickonservice.cpp \
    traymanager.cpp \
    utils/constutil.cpp \
    utils/envUtil.cpp \
    utils/processutil.cpp \
    utils/yamlutil.cpp \
    hardware/hardwareinfo.cpp \
    httprequest/zhttprequest.cpp \
    zapplication.cpp

HEADERS += \
    base/gbus.h \
    base/glang.h \
    base/gparams.h \
    base/gscale.h \
    base/infoconfig.h \
    base/inforesult.h \
    base/runner.h \
    base/sendproxy.h \
    base/service.h \
    base/servicebase.h \
    configs/yaml2stream.h \
    configs/yamlstream.h \
    base/zoffsetmenu.h \
    base/zpopupbutton.h \
    base/zpopupmenu.h \
    base/zwidget.h \
    controller.h \
    controls/apachesetting.h \
    controls/askconfirm.h \
    controls/askpassword.h \
    controls/closedialog.h \
    controls/globalcontrol.h \
    controls/infomessage.h \
    controls/logcontainer.h \
    controls/modifyaccount.h \
    controls/modifyport.h \
    controls/servicecontainer.h \
    controls/servicecontrol.h \
    controls/vcinstallask.h \
    runners/appinstaller.h \
    runners/backupruner.h \
    runners/envrunner.h \
    runners/modifyrunner.h \
    mainwindow.h \
    runners/serviceinstaller.h \
    servicehelper.h \
    services/apacheservice.h \
    services/groupservice.h \
    services/mysqlservice.h \
    services/productservice.h \
    services/quickonservice.h \
    traymanager.h \
    utils/constutil.h \
    utils/envutil.h \
    utils/processutil.h \
    utils/yamlutil.h \
    hardware/hardwareinfo.h \
    httprequest/zhttprequest.h \
    zapplication.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


win32:CONFIG(release, debug|release): LIBS += \
    -L$$OUT_PWD/../yaml/release/ \
    -lyaml \
    -lAdvapi32 \
    -lShell32 \
    -lPsapi \
    -lws2_32 \
    -lIphlpapi
else:win32:CONFIG(debug, debug|release): LIBS += \
    -L$$OUT_PWD/../yaml/debug/ \
    -lyaml \
    -lAdvapi32 \
    -lShell32 \
    -lPsapi \
    -lws2_32 \
    -lIphlpapi

INCLUDEPATH += $$PWD/../yaml
DEPENDPATH += $$PWD/../yaml

include(./qtsingleapplication/qtsingleapplication.pri)

RESOURCES +=
