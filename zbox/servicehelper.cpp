#include "servicehelper.h"

#include "utils/constutil.h"
#include "utils/processutil.h"
#include "base/gparams.h"
#include "controls/askpassword.h"
#include "utils/envutil.h"
#include "controls/vcinstallask.h"
#include "base/glang.h"

#include <QtDebug>

void ServiceHelper::autoSelectPort(Service *service,YamlStream *userConfig)
{
    if(service->state() != ConstUtil::SERVICE_UNKNOWN) return;
    if(service->fixPort() == true) return;

    QString port = service->port();
    if(ProcessUtil::isPortIdle(port) && GParams::instance()->existParam("PORT",port)) return;

    port = service->initPort();

    QList<int> range = service->autoPorts();
    if(range.length() !=2)
    {
        range.clear();
        range.append(port.toInt());
        range.append(65536);
    }

    int min = range[0];
    int max = range[1];

    QString newPort;

    for(int i=min;i<=max;i++)
    {
        QString nowPort = QString::number(i);
        if(ProcessUtil::isPortIdle(nowPort) == true && GParams::instance()->existParam("PORT",nowPort)==false)
        {
            newPort = nowPort;
            break;
        }
    }

    if(newPort.isEmpty() == false)
    {
        GParams::instance()->addParam(service->type().toUpper()+"_PORT",newPort);
        userConfig->setString(service->type()+".port",newPort);

        userConfig->save();
    }
}

void ServiceHelper::autoSelectPort(QList<Service*> serviceList,YamlStream *userConfig)
{
    foreach(Service *service,serviceList)
    {
        autoSelectPort(service,userConfig);
    }
}

void ServiceHelper::checkMysqlPassword(YamlStream *userConfig, QWidget *parent)
{
    QString mysqlPasswordKey = "MYSQL_PASSWORD";
    QString mysqlPassword = GParams::instance()->getParam(mysqlPasswordKey);

    if(mysqlPassword.isEmpty()) return;

    QRegExp passwordRx(ConstUtil::RX_PASSWORD);
    if(!passwordRx.exactMatch(mysqlPassword))
    {
        AskPassword *ask = new AskPassword(mysqlPasswordKey,mysqlPassword);
        ask->show();
        ask->exec();

        if(ask->changed())
        {
            userConfig->setString("mysql.password",ask->newPassword());
            userConfig->save();
        }
    }
}

void ServiceHelper::checkMysqlPassword(Service *service, YamlStream *userConfig, QWidget *parent)
{
    if(service->type() != "mysql" || service->state() != ConstUtil::SERVICE_UNKNOWN)
        return;

    checkMysqlPassword(userConfig,parent);
}

void ServiceHelper::checkMysqlPassword(QList<Service *> serviceList, YamlStream *userConfig, QWidget *parent)
{
    bool canChangePassword = false;
    foreach(Service *service,serviceList)
    {
        if(service->type() == "mysql" && service->state() == ConstUtil::SERVICE_UNKNOWN)
        {
            canChangePassword = true;
            break;
        }
    }

    if(canChangePassword == true)
        checkMysqlPassword(userConfig,parent);
}

bool ServiceHelper::checkVCEnvironment(Service *service,YamlStream *userConfig)
{
    if(userConfig->getboolean("skipVCCheck",false) == true || service->vcRumtime().isEmpty() == true)
        return true;

    bool VCInstalled = IsVCInstalled(service->vcRumtime());

    if(VCInstalled == true)
        return true;

    VCInstallAsk *askWin = new VCInstallAsk(service->vcRumtime(),"window.vcAskTitle","window.vcAskContent");
    askWin->show();
    askWin->exec();

    QString result = askWin->result();

    if(result == "cancel")
        return false;

    QString exePath = EnvUtil::getPath("zbox/vc_redist." + GParams::instance()->getOsArch() + ".exe");

    InfoResult exeResult = ProcessUtil::excuteCommand(exePath);
    if(exeResult.error == false)
        return true;

    GLang lang;

    QString exeMsg = exeResult.output;
    if(exeMsg.isEmpty())
        exeMsg =  lang.tlng("window.errorVCInstall");
    else
        exeMsg +=  "." + lang.tlng("window.errorVCInstall");

    VCInstallAsk *errorWin = new VCInstallAsk(service->vcRumtime(),"window.vcAskTitle",exeMsg);
    errorWin->show();
    errorWin->exec();

    if(errorWin->result() == "confirm")
    {
        userConfig->setBoolean("skipVCCheck",true);
        userConfig->save();

        return true;
    }
    else
        return false;
}

bool ServiceHelper::IsVCInstalled(QString vcRumtime)
{
    QString batPath = EnvUtil::getPath("zbox/vc_detector_" + GParams::instance()->getOsArch() + ".bat");
    InfoResult infoResult = ProcessUtil::excuteShell(batPath);

    QStringList msgList = infoResult.output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    return infoResult.output.indexOf("INSTALLED") >=0;
}
