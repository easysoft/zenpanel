/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
