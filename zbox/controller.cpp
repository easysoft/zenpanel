#include "controller.h"

#include "mainwindow.h"
#include "utils/envutil.h"
#include "utils/yamlutil.h"
#include "base/gparams.h"
#include "utils/constutil.h"
#include "runners/appinstaller.h"
#include "utils/processutil.h"
#include "runners/backupruner.h"
#include "runners/modifyrunner.h"
#include "controls/infomessage.h"
#include "servicehelper.h"
#include "base/glang.h"
#include "runners/serviceinstaller.h"
#include "services/quickonservice.h"

#include "spdlogwrapper.hpp"

#include <QApplication>
#include <QMessageBox>
#include <QMap>
#include <QScreen>
#include <QPoint>
#include <QCursor>
#include <QVariant>

Controller::Controller()
{
    computeScale();

    if(checkApplicationPath() == false)
    {
        QString title = "错误提示";
        QString infoStr = "无法安装启动服务，因为当前程序所在路径包含中文等特殊字符。程序所在路径必须仅包含数字、大小写字母或下划线，请修改路径后重试.";
        InfoMessage *infoMsg = new InfoMessage(title,infoStr);
        infoMsg->show();

        return;
    }

    initInitConfig();
    initUserConfig();
    initLangConfig();

    createMainProduct();
    createSubProductList();
    createSerivceList();

    askOsArch();

    QList<Service*> allList;
    allList.append(m_mainProduct);
    foreach(ProductService *sub,m_subProductList) allList.append(sub);
    foreach(Service *service,m_serviceList) allList.append(service);

    foreach(Service *base, allList)
    {
        if(ServiceHelper::checkVCEnvironment(base,m_userConfig) == false)
        {
            return;
        }
    }

    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);
    updateAppState();
    L_TRACE("{0} @ {1} extractParamValues", __FUNCTION__, __LINE__);
    extractParamValues();
    
    L_TRACE("{0} @ {1} MainWindow", __FUNCTION__, __LINE__);
    m_mainWin = new MainWindow(this);
    
    // 这里需要用 m_mainWin
    setupServiceSignal();

    m_mainWin->show();
}

QString Controller::themeName()
{
    QString defaultTheme = m_iniConfig->getString("common.theme","blue");
    QString userTheme = m_userConfig->getString("theme");

    return userTheme.isEmpty() == false ? userTheme : defaultTheme;
}

void Controller::setThemeName(QString _themeName)
{
    m_userConfig->setString("theme",_themeName);
    m_userConfig->save();
}

QString Controller::langName()
{
    return m_userConfig->getString("lang","zh-cn");
}

void Controller::setLangName(QString _langName)
{
    m_userConfig->setString("lang",_langName);
    m_userConfig->save();
}

QString Controller::logoPath()
{
    QString path = EnvUtil::getPath("zbox/res/theme/" + themeName() + "/images/logo.png");

    return path;
}

QString Controller::toolTitle()
{
    return m_iniConfig->getString("common.title","window.title");
}

QString Controller::toolVersion()
{
    return m_iniConfig->getString("common.version","1.0.0");
}

QString Controller::customStrParam(QString propPath)
{
    return m_userConfig->getString(propPath);
}

bool Controller::customBoolParam(QString propPath, bool defaultValue)
{
    return m_userConfig->getboolean(propPath,defaultValue);
}

QString Controller::mainProductCode()
{
    return m_mainProduct->code();
}

QString Controller::mainProductLaunch()
{
    return m_mainProduct->launch();
}

QString Controller::applicationState() const
{
    return m_applicationState;
}

ProductService *Controller::mainProduct() const
{
    return m_mainProduct;
}

QList<ProductService *> Controller::subProductList() const
{
    return m_subProductList;
}

QList<Service*> Controller::serviceList() const
{
    return m_serviceList;
}

Service *Controller::apacheService()
{
    for(int i=0;i<m_serviceList.length();i++)
    {
        Service *service = m_serviceList[i];
        if(service->type().toLower() == "apache")
            return service;
    }

    return nullptr;
}

QList<Service*> Controller::queryServiceList(bool includeNoService,bool includeIgnore)
{
    QList<Service*> list;
    foreach(Service *service,m_serviceList)
    {
        if(service->isWindowService() == false)
        {
            if(includeNoService == true)
                list.append(service);
        }
        else if(service->ignore() == true)
        {
            if(includeIgnore == true)
                list.append(service);
        }
        else
        {
            list.append(service);
        }
    }

    return list;
}

YamlStream *Controller::userConfig() const
{
    return m_userConfig;
}

void Controller::initInitConfig()
{
    QString userConfigPath = EnvUtil::getPath("etc/zbox/config.yml");
    QString defaultConfigPath = EnvUtil::getPath("zbox/config.yml");
    m_iniConfig = new Yaml2Stream(userConfigPath,defaultConfigPath);
}

void Controller::initUserConfig()
{
    QString filePath = EnvUtil::getPath("tmp/zbox.yml");
    m_userConfig = new YamlStream(filePath);
}

void Controller::initLangConfig()
{
    QString defaultLang = m_iniConfig->getString("common.lang");
    QString userLang = m_userConfig->getString("lang");

    QString lang = userLang.isEmpty() ? defaultLang : userLang;

    loadLang(lang);
}

void Controller::createMainProduct()
{
     YAML::Node currentNode = m_iniConfig->getCurrent()->getNode("app");
     YAML::Node defaultNode = m_iniConfig->getDefault()->getNode("app");
     Yaml2Stream *config = new Yaml2Stream(currentNode,defaultNode);

     m_mainProduct = new ProductService(this,config,config->getString("code"));
}

void Controller::createSubProductList()
{
    m_subProductList.clear();

    YAML::Node currentEntries = m_iniConfig->getCurrent()->getNode("app.entries");
    YAML::Node defaultEntries = m_iniConfig->getDefault()->getNode("app.entries");

    QMap<QString,QList<YAML::Node>> codeMaps;

    int defaultCount = defaultEntries.size();
    for(int i=0;i<defaultCount;i++)
    {
        YAML::Node entry = defaultEntries[i];
        QString key = YAMLUtil::toQString(entry["code"].as<std::string>());

        if(codeMaps.contains(key) == false)
        {
            QList<YAML::Node> list;
            list.append(YAML::Node());
            list.append(YAML::Node());

            codeMaps.insert(key,list);
        }

        codeMaps[key][1] = entry;
    }

    int currentCount = currentEntries.size();
    for(int i=0;i<currentCount;i++)
    {
        YAML::Node sub = currentEntries[i];

        QString key = YAMLUtil::toQString(sub["code"].as<std::string>());

        if(codeMaps.contains(key) == false)
        {
            QList<YAML::Node> list;
            list.append(YAML::Node());
            list.append(YAML::Node());

            codeMaps.insert(key,list);
        }

        codeMaps[key][0] = sub;
    }

    QList<QString> keys = codeMaps.keys();
    for(int i=0;i<keys.length();i++)
    {
        QList<YAML::Node> list = codeMaps[keys[i]];
        Yaml2Stream *config = new Yaml2Stream(list[0],list[1]);

        ProductService *service = new ProductService(this,config,keys[i]);
        m_subProductList.append(service);
    }
}

void Controller::createSerivceList()
{
    m_serviceList.clear();

    YAML::Node currentServiceList = m_iniConfig->getCurrent()->getNode("service");
    YAML::Node defaultServiceList = m_iniConfig->getDefault()->getNode("service");

    QMap<QString,QList<YAML::Node>> serviceMaps;

    int defaultCount = defaultServiceList.size();
    for(int i=0;i<defaultCount;i++)
    {
        YAML::Node sub = defaultServiceList[i];
        QList<QString> members = YAMLUtil::mapKeys(sub);
        QString key = members[0].toLower();

        if(serviceMaps.contains(key) == false)
        {
            QList<YAML::Node> list;
            list.append(YAML::Node());
            list.append(YAML::Node());

            serviceMaps.insert(key,list);
        }

        serviceMaps[key][1] = sub;
    }

    QList<QString> orderKeys;
    int currentCount = currentServiceList.size();
    for(int i=0;i<currentCount;i++)
    {
        YAML::Node sub = currentServiceList[i];
        QList<QString> members = YAMLUtil::mapKeys(sub);
        QString key = members[0].toLower();

        orderKeys.append(key);

        if(serviceMaps.contains(key) == false)
        {
            QList<YAML::Node> list;
            list.append(YAML::Node());
            list.append(YAML::Node());

            serviceMaps.insert(key,list);
        }

        serviceMaps[key][0] = sub;
    }

    QList<QString> keys = serviceMaps.keys();
    foreach(QString key,keys)
    {
        if(orderKeys.contains(key) == false)
            orderKeys.append(key);
    }

    for(int i=0;i<orderKeys.length();i++)
    {
        QList<YAML::Node> list = serviceMaps[orderKeys[i]];

        Yaml2Stream *config = new Yaml2Stream(list[0],list[1]);

        if(config->getboolean("enabled",true) == false) continue;

        QString key = orderKeys[i];

        YAML::Node serviceNameNode = config->getNode("serviceName");
        if(serviceNameNode.Type() == YAML::NodeType::Sequence)
        {
            GroupService *serivce = new GroupService(this,config,key);
            m_serviceList.append(serivce);
        }
        else
        {
            if(key == "apache")
            {
                ApacheService *serivce = new ApacheService(this,config,key);
                m_serviceList.append(serivce);
            }
            else if (key == "mysql")
            {
                MysqlService *serivce = new MysqlService(this,config,key);
                m_serviceList.append(serivce);
            }
            else if (key == "quickon")
            {
                QuickOnService *serivce = new QuickOnService(this,config,key);
                m_serviceList.append(serivce);
            }
            else
            {
                Service *serivce = new Service(this,config,key);
                m_serviceList.append(serivce);
            }
        }
    }
}

void Controller::extractParamValues()
{
    GParams::instance()->addParam("ROOT_PATH",EnvUtil::getRootPath());
    GParams::instance()->addParam("APP_PATH",EnvUtil::getPath("app/"));
    GParams::instance()->addParam("BACKUP_PATH",EnvUtil::getPath("backup/"));
    GParams::instance()->addParam("SERVICE_PATH",EnvUtil::getPath("bin/"));
    GParams::instance()->addParam("CONFIG_TPL_PATH",EnvUtil::getPath("etc/"));
    GParams::instance()->addParam("LOG_PATH",EnvUtil::getPath("tmp/logs/"));
    GParams::instance()->addParam("SESSION_PATH",EnvUtil::getPath("tmp/session/"));


    foreach(Service *service,m_serviceList)
        service->extractParamValues();

    for(ProductService *service : m_subProductList)
        service->extractParamValues();

    m_mainProduct->extractParamValues();
}

void Controller::updateAppState(bool forcedQuery)
{
    L_TRACE("{0} @ {1}", __FUNCTION__, __LINE__);
    if(forcedQuery == true)
    {
        foreach(Service *service,m_serviceList)
            service->refreshState();
    }

    int start_pending = 0;
    int running = 0;
    int paused = 0;
    int stop_pending = 0;
    int stopped = 0;
    int unknown = 0;

    QList<Service*> serviceList = queryServiceList(false,false);

    foreach(Service *service,serviceList)
    {
        QString iState = service->state();
        if(iState == ConstUtil::U_SERVICE_START_PENDING)
            start_pending ++;
        else if(iState == ConstUtil::U_SERVICE_RUNNING)
            running ++;
        else if(iState == ConstUtil::U_SERVICE_PAUSED)
            paused ++;
        else if(iState == ConstUtil::U_SERVICE_STOP_PENDING)
            stop_pending ++;
        else if(iState == ConstUtil::U_SERVICE_STOPPED)
            stopped ++;
        else if(iState == ConstUtil::U_SERVICE_UNKNOWN)
            unknown ++;
    }

    if(running > 0 && running == serviceList.length())
        m_applicationState = ConstUtil::TOTAL_INSTALLED;
    else
        m_applicationState = ConstUtil::TOTAL_NOTINSTALLED;
}

void Controller::onAppStateChanged()
{
    //updateAppState(false);
    m_mainWin->onAppStateChanged();
}

bool Controller::isPreInstalled()
{
    return true;
    bool apacheInstalled = false;
    bool mysqlInstalled = false;

    for(int i=0;i<m_serviceList.length();i++)
    {
        Service *service = m_serviceList[i];
        if(service->type().toLower() == "apache" && service->queryState() == ConstUtil::U_SERVICE_RUNNING)
            apacheInstalled = true;

        if(service->type().toLower() == "mysql" && service->queryState() == ConstUtil::U_SERVICE_RUNNING)
            mysqlInstalled = true;
    }

    return apacheInstalled && mysqlInstalled;
}

bool Controller::checkApplicationPath()
{    
    QRegExp rx("^[a-zA-Z\\d_\\\\/:]*$");
    if(rx.exactMatch(EnvUtil::getRootPath()) == false)
    {
        return false;
    }

    return true;
}

void Controller::setupServiceSignal()
{
    for (auto service : m_serviceList)
        service->SetupSignal();
}

void Controller::computeScale()
{
    QPoint mousePos = QCursor::pos();

    QScreen *current = QApplication::primaryScreen();

    QList<QScreen*> oList = QApplication::screens();
    for(QScreen* screen:oList)
    {
        QRect mRect = screen->geometry();
        if(mRect.contains(mousePos))
        {
            current = screen;
            break;
        }
    }

    float dpi = current->logicalDotsPerInch();

    float pixel120ToMM = 1.0/96.0 * 2.54 * 10.0;
    float MMToDpiPixel = 1.0/2.54*dpi/10.0;

    float scale = pixel120ToMM * MMToDpiPixel * current->devicePixelRatio();

    GParams::instance()->setScale(scale);
}

void Controller::askOsArch()
{
    QString checkBat = EnvUtil::getPath("zbox/check_os.bat");
    InfoResult InfoResult = ProcessUtil::excuteShell(checkBat);

    QString result = "x86";
    if(InfoResult.output.indexOf("64")>=0)
        result = "x64";

    GParams::instance()->setOsArch(result);
}

void Controller::loadLang(QString langKey)
{
    QString currentLngPath = EnvUtil::getPath("etc/zbox/lang/"+langKey + ".yml");
    QString defaultLngPath = EnvUtil::getPath("zbox/lang/"+langKey+".yml");

    Yaml2Stream *stream = new Yaml2Stream(currentLngPath,defaultLngPath);

    GParams::instance()->setLangName(langKey);
    GParams::instance()->setLang(stream);
}

void Controller::oneSetup()
{
    AppInstaller *installer = new AppInstaller(m_mainProduct,&m_subProductList,&m_serviceList);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onOneInstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    m_applicationState = ConstUtil::TOTAL_INSTALLING;
    m_mainWin->onAppStateChanged();

    installer->startApplication();
}

void Controller::oneStop()
{
    AppInstaller *installer = new AppInstaller(m_mainProduct,&m_subProductList,&m_serviceList);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onOneStopFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->stopApplication();
}

void Controller::reStart(Service *service)
{
    ServiceInstaller *installer = new ServiceInstaller(service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onOneInstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onServiceStateChanged(QString)));

    installer->reinstallSingleService();
}

void Controller::reStart()
{
    AppInstaller *installer = new AppInstaller(m_mainProduct,&m_subProductList,&m_serviceList);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onOneInstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    m_applicationState = ConstUtil::TOTAL_INSTALLING;
    m_mainWin->onAppStateChanged();

    installer->reStartApplication();
}

void Controller::oneUninstall()
{
    AppInstaller *installer = new AppInstaller(m_mainProduct,&m_subProductList,&m_serviceList);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onOneUninstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->uninstallApplication();
}

void Controller::appendMsg(MsgResult msg)
{
    m_mainWin->appendMsg(msg);
}

bool Controller::autoSelectPort(Service *service)
{
    QList<Service*> serviceList;
    if(service != nullptr)
        serviceList.append(service);
    else
        serviceList = m_serviceList;

    QStringList selfReadyPort;
    bool portAutoChanged = false;

    foreach(Service *service,serviceList)
    {
        if(service->state() != ConstUtil::U_SERVICE_UNKNOWN) continue;
        if(service->fixPort() == true) continue;

        QString port = service->port();
        if(ProcessUtil::isPortIdle(port) && selfReadyPort.contains(port)==false) continue;

        QList<int> range = service->autoPorts();
        if(range.length() !=2)
        {
            range.clear();
            range.append(port.toInt());
            range.append(65536);
        }

        int min = range[0];
        int max = range[1];

        QString selfPort;

        for(int i=min;i<=max;i++)
        {
            QString nowPort = QString::number(i);
            if(ProcessUtil::isPortIdle(nowPort) == true && selfReadyPort.contains(nowPort)==false)
            {
                selfPort = nowPort;
                break;
            }
        }

        if(selfPort.isEmpty())
        {
            return false;
        }

        selfReadyPort.append(selfPort);
        portAutoChanged = true;

        GParams::instance()->addParam(service->type().toUpper()+"_PORT",selfPort);
        m_userConfig->setString(service->type()+".port",selfPort);
    }

    if(portAutoChanged)
        m_userConfig->save();

    return true;
}

bool Controller::enableBackup()
{
    return m_mainProduct->backupEnabled();
}

void Controller::executeBackup()
{
    BackupRuner *runer = new BackupRuner(m_mainProduct);

    connect(runer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReceiveMsg(QVariant)));

    runer->start();
}

void Controller::applyInitConfig()
{
    ModifyRunner *runner = new ModifyRunner(m_mainProduct,m_subProductList,m_serviceList);
    runner->replaceConfigAll();
}

QList<MsgResult> Controller::basicsMsgList()
{
    QList<MsgResult> resultList;

    MsgResult titleResult;
    titleResult.error = false;
    titleResult.output = "basics.title";
    titleResult.placeholder1 = toolVersion();
    resultList.append(titleResult);

    foreach(Service *service, m_serviceList)
    {
        MsgResult serviceResult;
        serviceResult.error = false;
        serviceResult.type = service->type();
        serviceResult.output = "basics.serviceVersion";
        serviceResult.placeholder1 = service->title();

        QString version = ProcessUtil::exeVersion(service->versionCommand());
        if(version.isEmpty())
            version = service->version();

        serviceResult.placeholder2 = version;

        resultList.append(serviceResult);
    }

    MsgResult proResult;
    proResult.error = false;
    proResult.output = "basics.productVersion";
    proResult.placeholder1 = m_mainProduct->version();
    resultList.append(proResult);

    MsgResult dirResult;
    dirResult.error = false;
    dirResult.output = "basics.currentDir";
    dirResult.placeholder1 = EnvUtil::getRootPath();
    resultList.append(dirResult);

    return resultList;
}

Yaml2Stream *Controller::iniConfig() const
{
    return m_iniConfig;
}

void Controller::onReceiveMsg(QVariant msg)
{
    MsgResult result = msg.value<MsgResult>();
    appendMsg(result);
}

void Controller::onOneInstallFinished(bool result)
{
    updateAppState(false);
    m_mainWin->onAppStateChanged();
}

void Controller::onOneStopFinished(bool result)
{
    updateAppState(false);
    m_mainWin->onAppStateChanged();
}

void Controller::onOneUninstallFinished(bool result)
{
    updateAppState(false);
    m_mainWin->onAppStateChanged();
}

void Controller::onStateChanged(QString typeAndState)
{
    m_mainWin->onServiceStateChanged(typeAndState);
}

void Controller::onServiceStateChanged(QString typeAndState)
{
    m_mainWin->onAppStateChanged();
    m_mainWin->onServiceStateChanged(typeAndState);
}

void Controller::toActiveWindow(const QString &)
{
    m_mainWin->raise();
    m_mainWin->activateWindow();
    m_mainWin->show();
}

