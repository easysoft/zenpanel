/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef APACHESERVICEOPER_H
#define APACHESERVICEOPER_H

#include "base/service.h"

class ApacheService : public Service
{
public:
    ApacheService(Controller *controllor,Yaml2Stream *config,QString type);

    QString versionCommand();
    void extractParamValues();    

    InfoResult changePort(QString newPort);
    InfoResult changeUser(QString newAccount, QString newPassword);
};

#endif // APACHESERVICEOPER_H
