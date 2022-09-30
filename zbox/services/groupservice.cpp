#include "groupservice.h"

#include "qyaml.h"
#include "services/apacheservice.h"
#include "services/mysqlservice.h"
#include "services/quickonservice.h"
#include "base/servicebase.h"
#include "utils/yamlutil.h"
#include "configs/yaml2stream.h"
#include "utils/constutil.h"
#include "base/sendproxy.h"

#include <QtMath>

GroupService::GroupService(Controller *controllor,Yaml2Stream *config,QString type):
    Service(controllor,config,type)
{
    createMembers();
}

bool GroupService::isFamilyType(QString type)
{
    foreach(Service *service,m_members)
    {
        if(service->type() == type)
            return true;
    }

    return false;
}

QString GroupService::serviceName()
{
    return "";
}

void GroupService::refreshState()
{
    m_state = queryState();
}

bool GroupService::isWindowService()
{
    foreach(Service *service,m_members)
    {
        if(service->isWindowService() == false)
            return false;
    }

    return true;
}

QString GroupService::queryState()
{
    QList<QString> stateIndexs = QList<QString>()
            <<ConstUtil::U_SERVICE_START_PENDING
            <<ConstUtil::U_SERVICE_RUNNING
            <<ConstUtil::U_SERVICE_PAUSED
            <<ConstUtil::U_SERVICE_STOP_PENDING
            <<ConstUtil::U_SERVICE_STOPPED
            <<ConstUtil::U_SERVICE_UNKNOWN;
    int lastIndex = 0;

    foreach(Service *service,m_members)
    {
        QString stateStr = service->queryState();
        int index = stateIndexs.indexOf(stateStr);
        lastIndex = qMax(lastIndex,index);
    }

    return stateIndexs[lastIndex];
}


bool GroupService::installConfig(bool ignoreBug, SendProxy *proxy)
{
    bool result = true;

    foreach(Service *service,m_members)
    {
        bool success = service->installConfig(false,proxy);

        if(success == false) result = false;

        if(result == false && ignoreBug == false)
            return false;
    }

    return result;
}

bool GroupService::excCommand(bool ignoreBug, SendProxy *proxy)
{
    bool result = true;

    foreach(Service *service,m_members)
    {
        bool success = service->excCommand(false,proxy);

        if(success == false) result = false;

        if(result == false && ignoreBug == false)
            return false;
    }

    return result;
}

bool GroupService::installServiceImpl(SendProxy *proxy)
{
    SendProxy *ignoreProxy = new SendProxy();

    QString selfServiceState = this->state();
    if(selfServiceState != ConstUtil::U_SERVICE_UNKNOWN)
    {
        return false;
    }

    foreach(Service *service,m_members)
    {
        QString serviceState = service->queryState();
        if(serviceState != ConstUtil::U_SERVICE_UNKNOWN)
            service->killService(ignoreProxy);

        sleepBetween();

        if(!service->installService(proxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::uninstallServiceImpl(SendProxy *proxy)
{
    foreach(Service *service,m_members)
    {
        QString serviceState = service->queryState();

        if(serviceState == ConstUtil::U_SERVICE_UNKNOWN) continue;

        if(!service->killService(proxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::startServiceImpl(SendProxy *proxy)
{
    SendProxy *ignoreProxy = new SendProxy();
    QString serviceState = this->state();
    if(serviceState != ConstUtil::U_SERVICE_STOPPED)

    {
        return false;
    }

    foreach(Service *service,m_members)
    {
        service->killService(ignoreProxy);
        sleepBetween();
        if(!service->installService(ignoreProxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::stopServiceImpl(SendProxy *proxy)
{
    foreach(Service *service,m_members)
    {
        QString serviceState = service->queryState();
        if(serviceState == ConstUtil::U_SERVICE_RUNNING)
        {

        }
        if(!service->stopService(proxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::killServiceImpl(SendProxy *proxy)
{
    foreach(Service *service,m_members)
    {
        if(!service->killService(proxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::restartServiceImpl(SendProxy *proxy)
{
    foreach(Service *service,m_members)
    {
        if(!service->restartService(proxy))
            return false;
    }

    this->refreshState();
    return true;
}

bool GroupService::lazyInstallServiceImpl(SendProxy *proxy)
{
    foreach(Service *service,m_members)
    {
        if(!service->lazyInstallService(proxy))
            return false;

        service->refreshState();
    }

    this->refreshState();
    return true;
}

void GroupService::extractParamValues()
{
    Service::extractParamValues();

    foreach(Service *service,m_members)
    {
        service->extractParamValues();
    }
}

void GroupService::createMembers()
{
    YAML::Node nodeList = m_config->getNode("serviceName");
    int listLength = nodeList.size();
    for(int i=0;i<listLength;i++)
    {
        YAML::Node node = nodeList[i];

        QList<QString> members = YAMLUtil::mapKeys(node);
        QString key = members[0].toLower();

        Yaml2Stream *config = new Yaml2Stream(node);

        if(key == "apache")
        {
            ApacheService *service = new ApacheService(m_ctr,config,key);
            service->setParent(this);
            m_members.append(service);
        }
        else if(key == "mysql")
        {
            MysqlService *service = new MysqlService(m_ctr,config,key);
            service->setParent(this);
            m_members.append(service);
        }
        else if (key == "quickon")
        {
            QuickOnService *service = new QuickOnService(m_ctr,config,key);
            service->setParent(this);
            m_members.append(service);
        }
        else
        {
            Service *service = new Service(m_ctr,config,key);
            service->setParent(this);
            m_members.append(service);
        }
    }
}
