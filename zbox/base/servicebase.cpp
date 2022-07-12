#include "servicebase.h"

#include <QDir>
#include <QFile>
#include <QUrl>
#include <QTextStream>
#include <QDesktopServices>

#include "utils/envutil.h"
#include "utils/yamlutil.h"
#include "base/gparams.h"
#include "controller.h"
#include "utils/constutil.h"
#include "utils/processutil.h"

using namespace std;

ServiceBase::ServiceBase(Controller *controllor,Yaml2Stream *config,QString type)
{
    this->m_parent = nullptr;

    this->m_ctr = controllor;
    this->m_config = config;

    this->m_type = type;

    initConfigList();
    initCommandList();
}

QString ServiceBase::type()
{
    return m_type;
}

QString ServiceBase::title()
{
    QString _title = m_config->getString("title");
    if(_title.isEmpty())
        _title = toCase(m_type);

    if(m_parent != nullptr)
    {
        return m_parent->title() + "." + _title;
    }

    return _title;
}

QString ServiceBase::version()
{
    return m_config->getString("version");
}

QString ServiceBase::sourcePath()
{
    QString path = m_config->getString("path",type() + "/");
    return EnvUtil::getPath("bin/" + path);
}

QString ServiceBase::serviceName()
{
    QString _serviceName = m_config->getString("serviceName");

    if(_serviceName.isEmpty() == true)
        return type();

    return _serviceName;
}

QString ServiceBase::exePath()
{
    QString pathStr = m_config->getString("exePath");
    QStringList strList = pathStr.split("|");

    return EnvUtil::joinPath(sourcePath(), strList[0]);
}

QString ServiceBase::exeName()
{
    QString fullPath = exePath();

    int index = fullPath.lastIndexOf("/");

    return fullPath.mid(index + 1);
}

QString ServiceBase::versionCommand()
{
    QString exefullPath = exePath();

    QString pathStr = m_config->getString("exePath");
    QStringList strList = pathStr.split("|");

    QString versionLine = strList.length() >=2 ?strList[1] : "--version";

    return exefullPath + " " + versionLine;
}

QString ServiceBase::launch()
{
    return m_config->getString("launch");
}

QString ServiceBase::log()
{
    return m_config->getString("log");
}

QString ServiceBase::port()
{
    QString configPort = m_config->getString("port");
    QString customPort = m_ctr->customStrParam(this->type()+".port");

    QString resultPort = customPort.isEmpty() ? configPort : customPort;

    return resultPort;
}

QString ServiceBase::initPort()
{
    return m_config->getString("port");
}

QList<int> ServiceBase::autoPorts()
{
    return  m_config->getIntList("autoPorts");
}

bool ServiceBase::fixPort()
{
    return m_config->getboolean("fixPort",false);
}

QString ServiceBase::vcRumtime()
{
    return m_config->getString("vcRumtime");
}

bool ServiceBase::ignore()
{
    return m_config->getboolean("ignore",false);
}

QString ServiceBase::account()
{
    QString configAccount = m_config->getString("account");
    QString userAccount = m_ctr->customStrParam(this->type()+".account");

    QString resultAccount = userAccount.isEmpty() ? configAccount : userAccount;

    return resultAccount;
}

QString ServiceBase::password()
{
    QString configPassword = m_config->getString("password");
    QString userPassword = m_ctr->customStrParam(this->type() + ".password");

    QString resultPassword = userPassword.isEmpty() ? configPassword : userPassword;

    return resultPassword;
}

QString ServiceBase::state()
{
    return m_state;
}

void ServiceBase::setState(QString &state)
{
    m_state = state;
}

bool ServiceBase::canChangePort()
{
    if(fixPort() == true)
        return false;

    QString configedPort = port();
    return  configedPort.isEmpty() == true ? false : true;
}

InfoResult ServiceBase::changePort(QString newPort)
{
    InfoResult result;
    result.error = false;

    return result;
}

bool ServiceBase::enableAuth()
{
    return m_ctr->customBoolParam(this->type() + ".enableAuth",true);
}

bool ServiceBase::needMakeAuth()
{
    if(enableAuth() == false)
        return false;

    QString mAccount = account();
    QString mPassword = password();

    if(mAccount.isEmpty() || mPassword.isEmpty())
        return false;

    return true;
}

bool ServiceBase::canChangeAccount()
{
    return true;
}

bool ServiceBase::canChangeUser()
{
    QString mAccount = account();
    QString mPassword = password();

    if(mAccount.isEmpty() || mPassword.isEmpty())
        return false;

    return true;
}

InfoResult ServiceBase::changeUser(QString newAccount, QString newPassword)
{
    InfoResult result;
    result.error = false;

    return result;
}

bool ServiceBase::isFamilyType(QString _type)
{
    return this->type() == _type;
}

QList<ConfigInfo> ServiceBase::configList()
{
    return m_configList;
}

QList<CommandInfo> ServiceBase::commandList()
{
    return m_commandList;
}

QString ServiceBase::getDefaultConfigTplPath(QString selfPath)
{
    if(selfPath.isEmpty())
        return "";

    QString configPath = EnvUtil::getPath("etc/" + selfPath);

    return configPath;
}

QString ServiceBase::getDefaultConfigPath(QString selfPath)
{
    if(selfPath.isEmpty())
        return "";

    return EnvUtil::joinPath(sourcePath(),selfPath);
}

bool ServiceBase::checkVersion()
{
    QString versionPath = sourcePath() + "/VERSION.txt";

    QFile file(versionPath);
    if(file.exists() == false)
        return true;

    QTextStream txtInput(&file);


    QString configVersion = version();
    QString fileVersion;

    if(!txtInput.atEnd())
    {
        fileVersion = txtInput.readLine();
        fileVersion = configVersion.trimmed();
    }

    if(configVersion.isEmpty() == false && fileVersion.isEmpty() == false && configVersion != fileVersion)
        return false;

    return true;
}

bool ServiceBase::checkSourcePath()
{
    QString nowPath = sourcePath();

    QDir dir(nowPath);
    if(dir.exists() == false)
        return false;

    QStringList files = dir.entryList();

    return files.length() > 0;
}

QString ServiceBase::initializeCheck()
{
    bool sourceFlag = checkSourcePath();
    if(sourceFlag == false)
        return title() + "的源码文件不存在(源码路径：" + sourcePath()+ ").";

    bool versionFlag = checkVersion();
    if(versionFlag == false)
        return title() + "的版本号不一致(配置与源码目录中的版版本号对比).";

    return "";
}

void ServiceBase::initConfigList()
{
    QList<YAML::Node> nodeList = YAMLUtil::getList(m_config->getNode("config"));

    for(int i=0;i<nodeList.length();i++)
    {
        YAML::Node item = nodeList[i];
        std::string tmpConfigStr = item.as<std::string>();
        QString configStr = YAMLUtil::toQString(tmpConfigStr);

        QStringList commandSegs = configStr.split("|");

        if(commandSegs.length() != 2) continue;

        ConfigInfo info;
        info.fromPath = commandSegs[0];
        info.toPath = commandSegs[1];

        m_configList.append(info);
    }
}

void ServiceBase::initCommandList()
{
    QList<YAML::Node> beforeList = YAMLUtil::getList(m_config->getNode("beforeCommand"));
    QList<YAML::Node> commandList = YAMLUtil::getList(m_config->getNode("serviceCommand"));
    QList<YAML::Node> afterList = YAMLUtil::getList(m_config->getNode("afterCommand"));

    for(int i=0;i<beforeList.length();i++)
    {
        YAML::Node item = beforeList[i];
        std::string tmpConfigStr = item.as<std::string>();
        QString configStr = YAMLUtil::toQString(tmpConfigStr);

        CommandInfo info;
        info.type = "before";
        info.content = configStr;

        m_commandList.append(info);
    }

    for(int i=0;i<commandList.length();i++)
    {        
        YAML::Node item = commandList[i];
        std::string tmpConfigStr = item.as<std::string>();
        QString configStr = YAMLUtil::toQString(tmpConfigStr);

        CommandInfo info;
        info.type = "installer";
        info.content = configStr;

        m_commandList.append(info);
    }

    for(int i=0;i<afterList.length();i++)
    {
        YAML::Node item = afterList[i];
        std::string tmpConfigStr = item.as<std::string>();
        QString configStr = YAMLUtil::toQString(tmpConfigStr);

        CommandInfo info;
        info.type = "after";
        info.content = configStr;

        m_commandList.append(info);
    }
}

QString ServiceBase::toCase(QString str)
{
    if(str.isEmpty() || str.length() <=1)
        return str;

    return str.mid(0,1).toUpper() + str.mid(1);
}

void ServiceBase::openBrowser()
{
    QString webPort = port();

    if(webPort.isEmpty())
        webPort = GParams::instance()->getParam("APACHE_PORT");

    QString launchPage = this->launch();

    QString url = "http://127.0.0.1:" + webPort + "/" + launchPage;
    if(webPort.isEmpty())
        url = "http://127.0.0.1/" + launchPage;

    QDesktopServices::openUrl(QUrl(url));
}

void ServiceBase::openLog()
{
    QString url = EnvUtil::getPath("tmp/logs/" + log());
    QDesktopServices::openUrl(QUrl(url));
}

bool ServiceBase::isWindowService()
{
    return serviceName() == "false" ? false : true;
}

void ServiceBase::refreshState()
{
    m_state = queryState();
}

bool ServiceBase::modifyUser()
{
    return false;
}

void ServiceBase::extractParamValues()
{
    QString mType = m_type.toUpper();

    GParams::instance()->addParam(mType+"_PATH",sourcePath());
    GParams::instance()->addParam(mType+"_SER_NAME",serviceName());
    GParams::instance()->addParam(mType+"_PORT",port());

    GParams::instance()->addParam(mType+"_ACCOUNT",account());
    GParams::instance()->addParam(mType+"_PASSWORD",password());

    GParams::instance()->addParam(mType+"_EXE",exePath());
}

QString ServiceBase::queryState()
{
    QString nowServiceName = serviceName();
    if(nowServiceName == "false")
        return  ConstUtil::SERVICE_RUNNING;
    else
        return  ProcessUtil::serviceState(nowServiceName);
}

ServiceBase *ServiceBase::getParent() const
{
    return m_parent;
}

void ServiceBase::setParent(ServiceBase *parent)
{
    m_parent = parent;
}



