/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "apacheservice.h"

#include "utils/envutil.h"
#include "base/gparams.h"
#include "controller.h"

ApacheService::ApacheService(Controller *controllor,Yaml2Stream *config,QString type):
    Service(controllor,config,type)
{

}

void ApacheService::extractParamValues()
{
    Service::extractParamValues();

    QString authFile = EnvUtil::joinPath(sourcePath(),"auth/.htaccess");
    GParams::instance()->addParam("AUTH_FILE",authFile);

    QString authConfig = "Require all granted";

    if(needMakeAuth() == true)
    {
        authConfig = "AuthName \"" + m_ctr->mainProductCode() + " runner authentication required\"";
        authConfig += EnvUtil::NEWLINE + "AuthType Basic";
        authConfig += EnvUtil::NEWLINE + "AuthUserFile \"" + authFile + "\"";
        authConfig += EnvUtil::NEWLINE + "Require valid-user";
    }

    GParams::instance()->addParam("AUTH_CONFIG",authConfig);
}

InfoResult ApacheService::changePort(QString newPort)
{
    GParams::instance()->addParam(this->type().toUpper()+"_PORT",newPort);

    m_ctr->userConfig()->setString(this->type()+".port",newPort);
    m_ctr->userConfig()->save();

    InfoResult result;
    result.error = false;

    return result;
}

InfoResult ApacheService::changeUser(QString newAccount, QString newPassword)
{
    GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT",newAccount);
    GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD",newPassword);

    InfoResult result;
    result.error = false;

    return result;
}

QString ApacheService::versionCommand()
{
    QString exefullPath = exePath();

    QString pathStr = m_config->getString("exePath");
    QStringList strList = pathStr.split("|");

    QString versionLine = strList.length() >=2 ?strList[1] : "-version";

    return exefullPath + " " + versionLine;
}
