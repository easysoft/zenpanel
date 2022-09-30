#include "mysqlservice.h"
#include "base/gparams.h"
#include "controller.h"
#include "utils/constutil.h"

MysqlService::MysqlService(Controller *controllor,Yaml2Stream *config,QString type):
    Service(controllor,config,type)
{

}

//QString MysqlServiceOper::account()
//{
//    if(m_state == ConstUtil::SERVICE_UNKNOWN)
//    {
//        QString defaultAccount = m_itemConfig->getString("account");
//        return defaultAccount;
//    }
//    else
//    {
//        QString configAccount = m_itemConfig->getString("account");
//        QString userAccount = m_ctr->customStrParam(this->type()+".account");

//        QString resultAccount = userAccount.isEmpty() ? configAccount : userAccount;

//        return resultAccount;
//    }
//}

//QString MysqlServiceOper::password()
//{
//    if(m_state == ConstUtil::SERVICE_UNKNOWN)
//    {
//        QString defaultPassword = m_itemConfig->getString("password");
//        return defaultPassword;
//    }
//    else
//    {
//        QString configPassword = m_itemConfig->getString("password");
//        QString userPassword = m_ctr->customStrParam(this->type() + ".password");

//        QString resultPassword = userPassword.isEmpty() ? configPassword : userPassword;

//        return resultPassword;
//    }
//}

QString MysqlService::account()
{
    QString configAccount = m_config->getString("account");
    QString userAccount = m_ctr->customStrParam(this->type()+".account");

    QString resultAccount = userAccount.isEmpty() ? configAccount : userAccount;

    return resultAccount;
}

QString MysqlService::password()
{
    QString configPassword = m_config->getString("password");
    QString userPassword = m_ctr->customStrParam(this->type() + ".password");

    QString resultPassword = userPassword.isEmpty() ? configPassword : userPassword;

    return resultPassword;
}

bool MysqlService::canChangeAccount()
{
    return false;
}

void MysqlService::extractParamValues()
{
    Service::extractParamValues();

    GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT_OLD",account());
    GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD_OLD",password());

    GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT",account());
    GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD",password());

    if(m_state == ConstUtil::U_SERVICE_UNKNOWN)
    {
        QString userAccount = m_ctr->customStrParam(this->type()+".account");
        QString userPassword = m_ctr->customStrParam(this->type() + ".password");

        if(userAccount.isEmpty() == false && userPassword.isEmpty() == false)
        {
            GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT",userAccount);
            GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD",userPassword);
        }
    }
}

InfoResult MysqlService::changePort(QString newPort)
{
    GParams::instance()->addParam(this->type().toUpper()+"_PORT",newPort);

    m_ctr->userConfig()->setString(this->type()+".port",newPort);
    m_ctr->userConfig()->save();

    InfoResult result;
    result.error = false;

    return result;
}


InfoResult MysqlService::changeUser(QString newAccount, QString newPassword)
{
    GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT_OLD",account());
    GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD_OLD",password());

    GParams::instance()->addParam(this->type().toUpper() + "_ACCOUNT",newAccount);
    GParams::instance()->addParam(this->type().toUpper() + "_PASSWORD",newPassword);

    InfoResult result;
    result.error = false;

    return result;
}
