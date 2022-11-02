#include "service.h"

#include "base/inforesult.h"
#include "utils/processutil.h"
#include "utils/constutil.h"
#include "base/gparams.h"
#include "utils/envutil.h"

#include "spdlogwrapper.hpp"

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QDebug>

Service::Service(Controller *controllor,Yaml2Stream *config,QString type):
    ServiceBase(controllor,config,type)
{
}

void Service::sleepBetween()
{
    QThread::msleep(100);
}

bool Service::installConfig(ConfigInfo info,SendProxy *proxy)
{
    QString fromPath = EnvUtil::replaceText(info.fromPath);
    QString toPath = EnvUtil::replaceText(info.toPath);

    if(fromPath.indexOf(":")<0)
        fromPath = this->getDefaultConfigTplPath(fromPath);
    else
        fromPath = EnvUtil::normalizePath(fromPath);

    if(toPath.indexOf(":")<0)
        toPath = this->getDefaultConfigPath(toPath);
    else
        toPath = EnvUtil::normalizePath(toPath);

    if(fromPath.isEmpty() == true || toPath.isEmpty() == true)
    {
        proxy->toSend(getErrorMsg("message.errorConfigNoPath",this->title(),this->port(),fromPath + "|" + toPath));
        return false;
    }

    QFile fromFile(fromPath);
    if(fromFile.exists() == false)
    {
        proxy->toSend(getErrorMsg("message.errorConfigTplNotExist",this->title(),this->port(),fromPath));
        return false;
    }

    QString toFolder=toPath.left(toPath.lastIndexOf("/"));
    QDir toDir(toFolder);
    if(toDir.exists() == false)
    {
        proxy->toSend(getErrorMsg("message.errorConfigNotExist",this->title(),this->port(),toPath));
        return false;
    }

    EnvUtil::replaceConfig(fromPath,toPath);

    return true;
}

bool Service::installConfig(bool ignoreBug,SendProxy *proxy)
{
    bool result = true;

    QList<ConfigInfo> infoList = this->configList();
    for(int i=0;i<infoList.length();i++)
    {
        bool success = installConfig(infoList[i],proxy);

        if(success == false) result = false;

        if(result == false && ignoreBug == false)
            return false;
    }

    return result;
}

bool Service::excCommand(CommandInfo info,SendProxy *proxy)
{
    QString commandStr = EnvUtil::replaceText(info.content);
    commandStr = EnvUtil::normalizePath(commandStr);

    qDebug()<<"-"<<this->type()<<"------------------------------";
    qDebug()<<"Config Command:"<<info.content;
    qDebug()<<"Real Command:"<<commandStr;

    InfoResult result = ProcessUtil::excuteCommand(commandStr);

    if(result.error == true)
    {
        QString output = commandStr;
        if(result.output.isEmpty() == false)
        {
            output += EnvUtil::NEWLINE + result.output;
        }

        proxy->toSend(getErrorMsg("message.errorCommand",this->title(),this->port(),output));
        return false;
    }

    return true;
}

bool Service::excCommand(bool ignoreBug,SendProxy *proxy)
{
    bool result = true;

    QList<CommandInfo> infoList = this->commandList();
    for(int i=0;i<infoList.length();i++)
    {
        bool success = excCommand(infoList[i],proxy);

        if(success == false) result = false;

        if(result == false && ignoreBug == false)
            return false;
    }

    return result;
}

MsgResult Service::getInfoMsg(QString msg, QString placeholder1, QString placeholder2,QString directOutput)
{
    MsgResult msgResult;

    msgResult.error = false;
    msgResult.output = msg;
    msgResult.type = this->type();
    msgResult.port = this->port();
    msgResult.placeholder1 = placeholder1;
    msgResult.placeholder2 = placeholder2;
    msgResult.directOutput = directOutput;

    return msgResult;
}

MsgResult Service::getSuccessMsg(QString msg, QString placeholder1, QString placeholder2,QString directOutput)
{
    MsgResult msgResult;

    msgResult.error = false;
    msgResult.output = msg;
    msgResult.type = this->type();
    msgResult.port = this->port();
    msgResult.placeholder1 = placeholder1;
    msgResult.placeholder2 = placeholder2;
    msgResult.directOutput = directOutput;

    return msgResult;
}

MsgResult Service::getErrorMsg(QString msg, QString placeholder1, QString placeholder2,QString directOutput)
{
    MsgResult msgResult;

    msgResult.error = true;
    msgResult.output = msg;
    msgResult.type = this->type();
    msgResult.port = this->port();
    msgResult.placeholder1 = placeholder1;
    msgResult.placeholder2 = placeholder2;
    msgResult.directOutput = directOutput;

    return msgResult;
}

bool Service::lazyInstallService(SendProxy *proxy)
{
    return lazyInstallServiceImpl(proxy);
}

bool Service::lazyInstallServiceImpl(SendProxy *proxy)
{
    QString selfNameserviceName =this->serviceName();
    QString selfServiceState = this->queryState();

    if(selfServiceState == ConstUtil::U_SERVICE_UNKNOWN)
    {
        return installService(proxy);
    }
    else
    {
        QString existBinaryPath = EnvUtil::normalizePath(ProcessUtil::serviceBinaryPath(selfNameserviceName));
        QString configBinaryPath = this->sourcePath();
        if(existBinaryPath.startsWith(configBinaryPath) == false)
        {
            bool uninstallResult = uninstallService(proxy);
            if(uninstallResult == false) return false;

            sleepBetween();

            return installService(proxy);
        }
        else
        {
            if(selfServiceState == ConstUtil::U_SERVICE_RUNNING)
            {
                return true;
            }
            else if(selfServiceState == ConstUtil::U_SERVICE_STOPPED)
            {
                return  startService(proxy);
            }
            else
            {
                bool killResult = killService(proxy);
                if(killResult == false) return false;

                sleepBetween();

                return  installService(proxy);
            }
        }
    }
}

bool Service::installService(SendProxy *proxy)
{
    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_INSTALL_ING);

    if(installServiceImpl(proxy))
    {
        //proxy->toSendState(this->type() + "|" + ConstUtil::STATE_INSTALL_SUCCESS);
        return true;
    }

    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_INSTALL_ERROR);
    return false;
}

bool Service::installServiceImpl(SendProxy *proxy)
{
    QString selfServiceState = this->queryState();
    if(selfServiceState != ConstUtil::U_SERVICE_UNKNOWN)
    {
        L_ERROR("%s : %s\n", __FUNCTION__, selfServiceState.toStdString().c_str());
        return false;
    }

    QString selfSourcePath = this->sourcePath();

    QDir dir(selfSourcePath);
    if(dir.exists() == false)
    {
        proxy->toSend(getErrorMsg("message.errorPathNotExist",this->title(),this->port()));
        return false;
    }

    bool configResult = installConfig(false,proxy);
    if(configResult == false)
    {
        L_ERROR("{0} : configResult", __FUNCTION__);
        return false;
    }

    for(int i=0; i<m_commandList.length();i++)
    {
        if(m_commandList[i].content.isEmpty() == true)
        {
            proxy->toSend(getErrorMsg("message.errorCommandNot", this->title(),this->port()));
            L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
            return false;
        }
    }

    for(int i=0; i<m_commandList.length();i++)
    {
        if(m_commandList[i].type != "before") continue;

        if(!excCommand(m_commandList[i],proxy))
        {
            L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
            return false;
        }

        sleepBetween();
    }

    for(int i=0; i<m_commandList.length();i++)
    {
        if(m_commandList[i].type != "installer") continue;

        if(!excCommand(m_commandList[i],proxy))
        {
            L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
            return false;
        }

        sleepBetween();
    }

    if(this->isWindowService() == true)
    {
        QString newServiceState = this->queryState();

        if(newServiceState == ConstUtil::U_SERVICE_STOPPED)
        {
            this->setState(newServiceState);

            proxy->toSend(getSuccessMsg("message.installSuccess",this->title(),this->port()));

            if(!startService(proxy))
            {
                L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
                return false;
            }

            sleepBetween();
        }
    }

    for(int i=0; i<m_commandList.length();i++)
    {
        if(m_commandList[i].type != "after") continue;

        if(!excCommand(m_commandList[i],proxy))
        {
            L_ERROR("{0} @ {1} FAILED", __FUNCTION__, __LINE__);
            return false;
        }

        sleepBetween();
    }

    return true;
}

bool Service::uninstallService(SendProxy *proxy)
{
    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_UNINSTALL_ING);

    if(uninstallServiceImpl(proxy))
    {
        proxy->toSendState(this->type() + "|" + ConstUtil::STATE_UNINSTALL_SUCCESS);
        return true;
    }

    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_UNINSTALL_ERROR);
    return false;
}

bool Service::uninstallServiceImpl(SendProxy *proxy)
{
    QString serviceState = this->state();
    if(serviceState != ConstUtil::U_SERVICE_STOPPED)
    {
        return false;
    }

    QString removeCommand = m_config->getString("removeCommand","sc delete %SER_NAME%");
    removeCommand = removeCommand.replace("%SER_NAME%",this->serviceName());
    InfoResult installResult = ProcessUtil::excuteCommand(removeCommand);

    sleepBetween();

    QString newServiceState = this->queryState();

    if(installResult.error == false && newServiceState == ConstUtil::U_SERVICE_UNKNOWN)
    {
        proxy->toSend(getSuccessMsg("message.uninstallSuccess",this->title(),this->port()));
        this->setState(ConstUtil::U_SERVICE_UNKNOWN);

        if(this->exeName().isEmpty() == false)
            ProcessUtil::excuteCommand("taskkill /f /im " + this->exeName());

        return true;
    }
    else
    {
        proxy->toSend(getErrorMsg("message.uninstallError",this->title(),this->port(),installResult.output));

        this->setState(newServiceState);

        return false;
    }
}

bool Service::startService(SendProxy *proxy)
{
    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_START_ING);

    if(startServiceImpl(proxy))
    {
        proxy->toSendState(this->type() + "|" + ConstUtil::STATE_START_SUCCESS);
        return true;
    }

    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_START_ERROR);
    return false;
}

bool Service::startServiceImpl(SendProxy *proxy)
{
    QString serviceState = this->state();
    if(serviceState != ConstUtil::U_SERVICE_STOPPED)
    {
        return false;
    }

    QString startCommand = m_config->getString("startCommand","net start");
    InfoResult installResult = ProcessUtil::excuteCommand(startCommand + " " + this->serviceName());

    sleepBetween();

    QString newServiceState = this->queryState();

    if(newServiceState == ConstUtil::U_SERVICE_RUNNING)
    {
        proxy->toSend(getSuccessMsg("message.startSuccess",this->title(),this->port()));
        this->setState(ConstUtil::U_SERVICE_RUNNING);

        return true;
    }
    else
    {
        proxy->toSend(getErrorMsg("message.startError",this->title(),this->port(),installResult.output));

        this->setState(newServiceState);

        return false;
    }
}

bool Service::stopService(SendProxy *proxy)
{
    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_STOP_ING);

    if(stopServiceImpl(proxy))
    {
        proxy->toSendState(this->type() + "|" + ConstUtil::STATE_STOP_SUCCESS);
        return true;
    }

    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_STOP_ERROR);
    return false;
}

bool Service::stopServiceImpl(SendProxy *proxy)
{
    QString serviceState = this->state();
    if(serviceState != ConstUtil::U_SERVICE_RUNNING)
    {
        return false;
    }

    InfoResult installResult = ProcessUtil::excuteCommand("net stop " + this->serviceName());

    sleepBetween();

    QString newServiceState = this->queryState();

    if(installResult.error == false && newServiceState == ConstUtil::U_SERVICE_STOPPED)
    {
        this->setState(ConstUtil::U_SERVICE_STOPPED);
        proxy->toSend(getSuccessMsg("message.stopSuccess",this->title(),this->port()));

        return true;
    }
    else
    {
        proxy->toSend(getErrorMsg("message.stopError",this->title(),this->port(),installResult.output));

        this->setState(newServiceState);

        return false;
    }
}

bool Service::killService(SendProxy *proxy)
{
    proxy->toSendState(this->type() + "|" + ConstUtil::STATE_UNINSTALL_ING);

    if(killServiceImpl(proxy))
    {
        proxy->toSendState(this->type() + "|" + ConstUtil::STATE_UNINSTALL_SUCCESS);
        return true;
    }

    proxy->toSendState(this->type()+"|"+ConstUtil::STATE_UNINSTALL_ERROR);
    return false;
}

bool Service::killServiceImpl(SendProxy *proxy)
{
    QString serviceState = this->state();
    if(serviceState == ConstUtil::U_SERVICE_UNKNOWN)
    {
        return false;
    }

    InfoResult result;

    QString pid = ProcessUtil::servicePID(this->serviceName());
    if(pid == "0")
    {
        QString removeCommand = m_config->getString("removeCommand","sc delete %SER_NAME%");
        removeCommand = removeCommand.replace("%SER_NAME%",this->serviceName());
        result = ProcessUtil::excuteCommand(removeCommand);
    }
    else
    {
        result = ProcessUtil::killService(pid);
        QString currentState = this->queryState();
        if(currentState == ConstUtil::U_SERVICE_STOPPED)
            result = ProcessUtil::excuteCommand("sc delete " + this->serviceName());
    }

    sleepBetween();

    QString newServiceState = this->queryState();

    if(newServiceState == ConstUtil::U_SERVICE_UNKNOWN)
    {
        this->setState(ConstUtil::U_SERVICE_UNKNOWN);
        proxy->toSend(getSuccessMsg("message.uninstallSuccess",this->title(),this->port()));

        if(this->exeName().isEmpty() == false)
            ProcessUtil::excuteCommand("taskkill /f /im " + this->exeName());

        return true;
    }
    else
    {
        proxy->toSend(getErrorMsg("message.uninstallError",this->title(),this->port(),result.output));

        this->setState(newServiceState);

        return false;
    }
}

bool Service::restartService(SendProxy *proxy)
{
    return restartServiceImpl(proxy);
}

bool Service::restartServiceImpl(SendProxy *proxy)
{
    QString serviceState = this->state();

    if(serviceState == ConstUtil::U_SERVICE_UNKNOWN)
    {
        return installService(proxy);
    }
    else if(serviceState == ConstUtil::U_SERVICE_RUNNING)
    {
        if(!stopService(proxy)) return false;
        sleepBetween();
        return startService(proxy);
    }
    else if(serviceState == ConstUtil::U_SERVICE_STOPPED)
    {
        return  startService(proxy);
    }
    else
    {
        if(!killService(proxy)) return false;
        sleepBetween();
        return installService(proxy);
    }
}





