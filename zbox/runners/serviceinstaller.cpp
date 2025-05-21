/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "serviceinstaller.h"

#include "utils/processutil.h"
#include "utils/constutil.h"

ServiceInstaller::ServiceInstaller(Service* service):Runner()
{
    this->m_service = service;
}

void ServiceInstaller::installSingleService()
{
    m_execute = "install";
    start();
}

void ServiceInstaller::reinstallSingleService()
{
    m_execute = "reinstall";
    start();
}

void ServiceInstaller::stopSingleSerivce()
{
    m_execute = "stop";
    start();
}

void ServiceInstaller::restartSingleService()
{
    m_execute = "restart";
    start();
}

void ServiceInstaller::uninstallSingleService()
{
    m_execute = "uninstall";
    start();
}

void ServiceInstaller::installSingleServiceImpl()
{
    QString serviceState = m_service->queryState();
    m_service->setState(serviceState);

    bool result = true;

    if(serviceState == ConstUtil::SERVICE_UNKNOWN)
    {
        result = m_service->installService(this);
    }

    toSendResult(result);
}

void ServiceInstaller::reInstallSingleServiceImpl()
{
    QString serviceState = m_service->queryState();
    m_service->setState(serviceState);

    if(serviceState == ConstUtil::SERVICE_UNKNOWN)
    {
        m_service->installService(this);
    }
    else if(serviceState == ConstUtil::SERVICE_RUNNING)
    {
        m_service->stopService(this);
        sleepBetween();
        m_service->uninstallService(this);
        sleepBetween();
        m_service->installService(this);
    }
    if(serviceState == ConstUtil::SERVICE_PAUSED ||
            serviceState == ConstUtil::SERVICE_START_PENDING ||
            serviceState == ConstUtil::SERVICE_STOP_PENDING)
    {
        m_service->killService(this);
        sleepBetween();
        m_service->installService(this);
    }
    else if(serviceState == ConstUtil::SERVICE_STOPPED)
    {
        m_service->uninstallService(this);
        sleepBetween();
        m_service->installService(this);
    }

    emit sendResult(true);
}

void ServiceInstaller::stopSingleSerivceImpl()
{
    QString serviceState = m_service->queryState();
    m_service->setState(serviceState);

    if(serviceState == ConstUtil::SERVICE_UNKNOWN ||
            serviceState == ConstUtil::SERVICE_STOPPED)
    {
        //No other processing is required
    }
    else if(serviceState == ConstUtil::SERVICE_RUNNING)
    {
        m_service->stopService(this);
    }
    else if(serviceState == ConstUtil::SERVICE_PAUSED ||
            serviceState == ConstUtil::SERVICE_START_PENDING ||
            serviceState == ConstUtil::SERVICE_STOP_PENDING)
    {
        m_service->killService(this);
    }

    emit sendResult(true);
}

void ServiceInstaller::restartSingleServiceImpl()
{
    QString serviceState = m_service->queryState();
    m_service->setState(serviceState);

    m_service->restartService(this);

    toSendResult(true);
}

void ServiceInstaller::uninstallSingleServiceImpl()
{
    QString serviceState = m_service->queryState();
    m_service->setState(serviceState);

    if(serviceState == ConstUtil::SERVICE_UNKNOWN)
    {
        //No other processing is required
    }
    else if(serviceState == ConstUtil::SERVICE_RUNNING)
    {
        m_service->stopService(this);
        sleepBetween();
        m_service->uninstallService(this);
    }
    else if(serviceState == ConstUtil::SERVICE_STOPPED)
    {
        m_service->uninstallService(this);
    }
    else if(serviceState == ConstUtil::SERVICE_PAUSED ||
            serviceState == ConstUtil::SERVICE_START_PENDING ||
            serviceState == ConstUtil::SERVICE_STOP_PENDING)
    {
        m_service->killService(this);
    }

    emit sendResult(true);
}

void ServiceInstaller::run()
{
    if(m_execute == "install")
        installSingleServiceImpl();
    else if(m_execute == "reinstall")
        reInstallSingleServiceImpl();
    else if(m_execute == "stop")
        stopSingleSerivceImpl();
    else if(m_execute == "uninstall")
        uninstallSingleServiceImpl();
    else if(m_execute == "restart")
        restartSingleServiceImpl();

    m_execute = "";
}
