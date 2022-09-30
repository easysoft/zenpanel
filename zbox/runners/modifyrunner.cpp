#include "modifyrunner.h"
#include "base/infoconfig.h"
#include "base/inforesult.h"
#include "utils/constutil.h"
#include "utils/processutil.h"
#include "base/gparams.h"

ModifyRunner::ModifyRunner(ProductService *mainOper,QList<ProductService*> subList,QList<Service*> serviceList,Service *service,YamlStream *userConfig):
    Runner()
{
    this->m_service = service;

    this->m_mainOper = mainOper;
    this->m_subList = subList;
    this->m_servieList = serviceList;

    this->m_userConfig = userConfig;
}

ModifyRunner::ModifyRunner(ProductService *mainOper,QList<ProductService*> subList,QList<Service*> serviceList):Runner()
{
    this->m_mainOper = mainOper;
    this->m_subList = subList;
    this->m_servieList = serviceList;
}

void ModifyRunner::startConfig()
{
    m_excute = "config";

    start();
}

void ModifyRunner::startPort(QString port)
{
    m_excute = "port";
    m_newPort = port;

    start();
}

void ModifyRunner::startUser(QString newAccount,QString newPassowrd)
{
    m_excute = "user";
    m_newAccount = newAccount;
    m_newPassword = newPassowrd;

    start();
}

bool ModifyRunner::replaceConfigAll()
{
    for(int i=0;i<m_servieList.length();i++)
    {
        Service *service = m_servieList.at(i);
        if(!service->installConfig(false,this)) return false;
    }

    if(!m_mainOper->installConfig(false,this)) return false;

    for(int i=0;i<m_subList.length();i++)
    {
        ProductService *subOper = m_subList.at(i);
        if(!subOper->installConfig(false,this)) return false;
    }

    return true;
}

void ModifyRunner::startConfigImpl()
{
    replaceConfigAll();
    emit modifySccess("");
}

void ModifyRunner::startPortImpl()
{
    QString serviceState = m_service->queryState();
    if(serviceState != ConstUtil::U_SERVICE_RUNNING)
    {
        toSend(m_service->getErrorMsg("message.modifyPortErrorNoRunning",m_service->title(),m_newPort));
        return;
    }

    toSend(m_service->getInfoMsg("message.modifingPort",m_service->title(),m_newPort));

    if(m_newPort == m_service->port())
    {
        toSend(m_service->getSuccessMsg("message.modifyPortSuccess",m_service->title(),m_newPort));
        return;
    }

    InfoResult result = m_service->changePort(m_newPort);

    if(result.error == false)
    {
        if(!replaceConfigAll())
        {
            toSend(m_service->getErrorMsg("message.modifyPortError",m_service->title(),m_newPort));
            return;
        }

        toSend(m_service->getSuccessMsg("message.modifyPortSuccess",m_service->title(),m_newPort));


        emit modifySccess(m_newPort);
    }
    else
    {
        toSend(m_service->getErrorMsg("message.modifyPortError",m_service->title(),m_newPort,result.output));
    }
}

void ModifyRunner::startUserImpl()
{
    QString serviceState = m_service->queryState();
    if(serviceState != ConstUtil::U_SERVICE_RUNNING)
    {
        toSend(m_service->getErrorMsg("message.modifyUserErrorNoRunning",m_service->title(),m_service->port()));
        return;
    }

    toSend(m_service->getInfoMsg("message.modifingUser",m_service->title(),m_service->port()));

    if(m_newAccount == m_service->account() && m_newPassword == m_service->password())
    {
        toSend(m_service->getSuccessMsg("message.modifyUserSuccess",m_service->title(),m_service->port()));
        return;
    }

    InfoResult result = m_service->changeUser(m_newAccount,m_newPassword);

    if(result.error == false)
    {
        if(!replaceConfigAll())
        {
            toSend(m_service->getErrorMsg("message.modifyUserError",m_service->title(),m_service->port()));
            return;
        }

        m_userConfig->setString(m_service->type()+".account",m_newAccount);
        m_userConfig->setString(m_service->type()+".password",m_newPassword);
        m_userConfig->save();

        toSend(m_service->getSuccessMsg("message.modifyUserSuccess",m_service->title(),m_service->port()));
        emit modifySccess(m_newPort);
    }
    else
    {
        toSend(m_service->getErrorMsg("message.modifyUserError",m_service->title(),m_service->port(),result.output));
    }
}

void ModifyRunner::run()
{
    if(m_excute == "port")
        startPortImpl();
    else if(m_excute == "user")
        startUserImpl();
    else if(m_excute == "config")
        startConfigImpl();

    m_excute = "";
}
