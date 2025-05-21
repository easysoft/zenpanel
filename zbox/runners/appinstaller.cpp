/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "appinstaller.h"

#include <QVariant>

#include "base/inforesult.h"
#include "utils/constutil.h"
#include "utils/processutil.h"

AppInstaller::AppInstaller(ProductService *mainProduct,QList<ProductService*> *subProductList,QList<Service*> *serviceList):
    Runner()
{
    this->m_mainProduct = mainProduct;
    this->m_subProductList = subProductList;
    this->m_servieList = serviceList;
}

void AppInstaller::startApplication()
{
    m_execute = "start";

    start();
}

void AppInstaller::stopApplication()
{
    m_execute = "stop";

    start();
}

void AppInstaller::uninstallApplication()
{
    m_execute = "uninstall";

    start();
}

void AppInstaller::reStartApplication()
{
    m_execute = "reinstall";

    start();
}

void AppInstaller::startApplicationImpl()
{
    toSend("message.startingApp");

    bool configResult = m_mainProduct->installConfig(false,this);
    bool commandResult = m_mainProduct->excCommand(false,this);

    if(configResult == false || commandResult == false)
    {
        toSend("message.startAppError",true);
        return;
    }

    for(int i=0;i<m_subProductList->length();i++)
    {
        ProductService *subProduct = m_subProductList->at(i);

        bool subConfigResult = subProduct->installConfig(false,this);
        bool subCommandResult = subProduct->excCommand(false,this);

        if(subConfigResult == false || subCommandResult == false)
        {
            toSend("message.startAppError",true);
            return;
        }
    }

    for(int i=0;i<m_servieList->length();i++)
    {
        Service *service = m_servieList->at(i);

        if(service->isWindowService() == false)
        {
            bool serviceConfigResult = service->installConfig(false,this);
            bool serviceCommandResult = service->excCommand(false,this);

            if(serviceConfigResult == false || serviceCommandResult == false)
            {
                toSend("message.startAppError",true);
                return;
            }
        }
        else
        {
            QString serviceState = service->queryState();
            if(serviceState == ConstUtil::SERVICE_PAUSED ||
                    serviceState == ConstUtil::SERVICE_STOP_PENDING ||
                    serviceState == ConstUtil::SERVICE_START_PENDING)
            {
                service->killService(this);
                sleepBetween();
            }

            service->lazyInstallService(this);
        }
    }

    bool result = isUnifiedState(ConstUtil::SERVICE_RUNNING);

    if(result == true)
        toSend("message.startAppSuccess");
    else
        toSend("message.startAppError",true);

    emit sendResult(result);
}

void AppInstaller::stopApplicationImpl()
{
    toSend("message.stopingApp");

    for(int i=0;i<m_servieList->length();i++)
    {
        Service *service = m_servieList->at(i);

        if(service->isWindowService() == false)
            continue;

        QString serviceState = service->queryState();
        service->setState(serviceState);

        if(serviceState == ConstUtil::SERVICE_RUNNING)
        {
            service->stopService(this);
        }
        else if(serviceState == ConstUtil::SERVICE_UNKNOWN ||
                serviceState == ConstUtil::SERVICE_STOPPED)
        {
            continue;
        }
        else if(serviceState == ConstUtil::SERVICE_START_PENDING ||
                serviceState == ConstUtil::SERVICE_STOP_PENDING ||
                serviceState == ConstUtil::SERVICE_PAUSED)
        {
            service->killService(this);
        }
    }

    bool result = true;
    for(int i=0;i<m_servieList->length();i++)
    {
        if(m_servieList->at(i)->isWindowService() == false)
            continue;

        QString state = m_servieList->at(i)->state();
        if(state == ConstUtil::SERVICE_RUNNING ||
                state == ConstUtil::SERVICE_PAUSED ||
                state == ConstUtil::SERVICE_START_PENDING ||
                state == ConstUtil::SERVICE_STOP_PENDING)
        {
            result = false;
            break;
        }
    }

    if(result == true)
        toSend("message.stopAppSuccess");
    else
        toSend("message.stopAppError",true);

    toSendResult(result);
}

void AppInstaller::uninstallApplicationImpl()
{
    toSend("message.uninstallingApp");

    for(int i=0;i<m_servieList->length();i++)
    {
        Service *service = m_servieList->at(i);

        if(service->isWindowService() == false)
            continue;

        QString serviceState = service->queryState();
        service->setState(serviceState);

        if(serviceState == ConstUtil::SERVICE_UNKNOWN)
        {
            continue;
        }
        else if(serviceState == ConstUtil::SERVICE_STOPPED)
        {
            service->uninstallService(this);
        }
        else if(serviceState == ConstUtil::SERVICE_RUNNING)
        {
            service->stopService(this);
            sleepBetween();
            service->uninstallService(this);
        }
        else if(serviceState == ConstUtil::SERVICE_START_PENDING ||
                serviceState == ConstUtil::SERVICE_STOP_PENDING ||
                serviceState == ConstUtil::SERVICE_PAUSED)
        {
            service->killService(this);
        }
    }

    bool result = isUnifiedState(ConstUtil::SERVICE_UNKNOWN);

    if(result == true)
        toSend("message.uninstallAppSuccess");
    else
        toSend("message.uninstallAppError",true);

    emit sendResult(result);
}

void AppInstaller::reStartApplicationImpl()
{
    stopApplicationImpl();

    SendProxy *sendProxy = new SendProxy();
    m_mainProduct->installConfig(true,sendProxy);


    for(int i=0;i<m_subProductList->length();i++)
    {
        ProductService *subProduct = m_subProductList->at(i);
        bool subConfigResult = subProduct->installConfig(true,sendProxy);
    }

    for(int i=0;i<m_servieList->length();i++)
    {
        Service *service = m_servieList->at(i);
        service->installConfig(true,sendProxy);
    }

    startApplicationImpl();
}

void AppInstaller::run()
{
    if(m_execute == "start")
        startApplicationImpl();
    else if(m_execute == "stop")
        stopApplicationImpl();
    else if(m_execute == "uninstall")
        uninstallApplicationImpl();
    else if(m_execute == "reinstall")
        reStartApplicationImpl();

    m_execute = "";
}

bool AppInstaller::isUnifiedState(QString state)
{
    bool flag = true;
    for(int i=0;i<m_servieList->length();i++)
    {
        Service *service = m_servieList->at(i);
        if(service->isWindowService() == false)
            continue;

        if(service->state() != state){
            flag = false;
            break;
        }
    }

    return flag;
}
