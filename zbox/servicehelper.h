/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef OPERHELPER_H
#define OPERHELPER_H

#include "base/service.h"
#include "configs/yamlstream.h"

#include <QWidget>

#include <QList>

class ServiceHelper
{
public:
    static void autoSelectPort(Service *service,YamlStream *userConfig);
    static void autoSelectPort(QList<Service*> serviceList,YamlStream *userConfig);

    static void checkMysqlPassword(YamlStream *userConfig,QWidget *parent=nullptr);
    static void checkMysqlPassword(Service *service,YamlStream *userConfig,QWidget *parent=nullptr);
    static void checkMysqlPassword(QList<Service*> serviceList,YamlStream *userConfig,QWidget *parent=nullptr);

    static bool checkVCEnvironment(Service *service,YamlStream *userConfig);
    static bool IsVCInstalled(QString vcRumtime);

private:
    ServiceHelper(){};
};

#endif // OPERHELPER_H
