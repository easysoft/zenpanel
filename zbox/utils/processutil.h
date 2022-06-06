#ifndef PROCESSUTIL_H
#define PROCESSUTIL_H

#include <QProcess>
#include <QProcessEnvironment>
#include <QMap>

#include "base/inforesult.h"
#include "base/servicebase.h"

class ProcessUtil
{
public:
    static InfoResult excuteShell(QString command);
    static InfoResult excuteCommand(QString command,QStringList params=QStringList());
    static QString serviceState(QString serviceName);
    static QString serviceBinaryPath(QString serviceName);
    static QString exeVersion(QString exePath);
    static QString servicePID(QString serviceName);

    static InfoResult killService(QString pid);

    static bool isPortIdle(QString port);

};

#endif // PROCESSUTIL_H
