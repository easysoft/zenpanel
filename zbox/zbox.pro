QT       += core gui

RC_FILE += uac.rc


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += \
    QT_DEPRECATED_WARNINGS \
    USE_SPDLOG_

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    spdlogwrapper.cpp \
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
    spdlog/async.h \
    spdlog/async_logger-inl.h \
    spdlog/async_logger.h \
    spdlog/cfg/argv.h \
    spdlog/cfg/env.h \
    spdlog/cfg/helpers-inl.h \
    spdlog/cfg/helpers.h \
    spdlog/common-inl.h \
    spdlog/common.h \
    spdlog/details/backtracer-inl.h \
    spdlog/details/backtracer.h \
    spdlog/details/circular_q.h \
    spdlog/details/console_globals.h \
    spdlog/details/file_helper-inl.h \
    spdlog/details/file_helper.h \
    spdlog/details/fmt_helper.h \
    spdlog/details/log_msg-inl.h \
    spdlog/details/log_msg.h \
    spdlog/details/log_msg_buffer-inl.h \
    spdlog/details/log_msg_buffer.h \
    spdlog/details/mpmc_blocking_q.h \
    spdlog/details/null_mutex.h \
    spdlog/details/os-inl.h \
    spdlog/details/os.h \
    spdlog/details/periodic_worker-inl.h \
    spdlog/details/periodic_worker.h \
    spdlog/details/registry-inl.h \
    spdlog/details/registry.h \
    spdlog/details/synchronous_factory.h \
    spdlog/details/tcp_client-windows.h \
    spdlog/details/tcp_client.h \
    spdlog/details/thread_pool-inl.h \
    spdlog/details/thread_pool.h \
    spdlog/details/udp_client-windows.h \
    spdlog/details/udp_client.h \
    spdlog/details/windows_include.h \
    spdlog/fmt/bin_to_hex.h \
    spdlog/fmt/bundled/args.h \
    spdlog/fmt/bundled/chrono.h \
    spdlog/fmt/bundled/color.h \
    spdlog/fmt/bundled/compile.h \
    spdlog/fmt/bundled/core.h \
    spdlog/fmt/bundled/format-inl.h \
    spdlog/fmt/bundled/format.h \
    spdlog/fmt/bundled/locale.h \
    spdlog/fmt/bundled/os.h \
    spdlog/fmt/bundled/ostream.h \
    spdlog/fmt/bundled/printf.h \
    spdlog/fmt/bundled/ranges.h \
    spdlog/fmt/bundled/xchar.h \
    spdlog/fmt/chrono.h \
    spdlog/fmt/compile.h \
    spdlog/fmt/fmt.h \
    spdlog/fmt/ostr.h \
    spdlog/fmt/ranges.h \
    spdlog/fmt/xchar.h \
    spdlog/formatter.h \
    spdlog/fwd.h \
    spdlog/logger-inl.h \
    spdlog/logger.h \
    spdlog/pattern_formatter-inl.h \
    spdlog/pattern_formatter.h \
    spdlog/sinks/android_sink.h \
    spdlog/sinks/ansicolor_sink-inl.h \
    spdlog/sinks/ansicolor_sink.h \
    spdlog/sinks/base_sink-inl.h \
    spdlog/sinks/base_sink.h \
    spdlog/sinks/basic_file_sink-inl.h \
    spdlog/sinks/basic_file_sink.h \
    spdlog/sinks/daily_file_sink.h \
    spdlog/sinks/dist_sink.h \
    spdlog/sinks/dup_filter_sink.h \
    spdlog/sinks/hourly_file_sink.h \
    spdlog/sinks/mongo_sink.h \
    spdlog/sinks/msvc_sink.h \
    spdlog/sinks/null_sink.h \
    spdlog/sinks/ostream_sink.h \
    spdlog/sinks/qt_sinks.h \
    spdlog/sinks/ringbuffer_sink.h \
    spdlog/sinks/rotating_file_sink-inl.h \
    spdlog/sinks/rotating_file_sink.h \
    spdlog/sinks/sink-inl.h \
    spdlog/sinks/sink.h \
    spdlog/sinks/stdout_color_sinks-inl.h \
    spdlog/sinks/stdout_color_sinks.h \
    spdlog/sinks/stdout_sinks-inl.h \
    spdlog/sinks/stdout_sinks.h \
    spdlog/sinks/syslog_sink.h \
    spdlog/sinks/systemd_sink.h \
    spdlog/sinks/tcp_sink.h \
    spdlog/sinks/udp_sink.h \
    spdlog/sinks/win_eventlog_sink.h \
    spdlog/sinks/wincolor_sink-inl.h \
    spdlog/sinks/wincolor_sink.h \
    spdlog/spdlog-inl.h \
    spdlog/spdlog.h \
    spdlog/stopwatch.h \
    spdlog/tweakme.h \
    spdlog/version.h \
    spdlogwrapper.hpp \
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

DISTFILES += \
    spdlog/fmt/bundled/fmt.license.rst
