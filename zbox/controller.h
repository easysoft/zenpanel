#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "configs/yamlstream.h"
#include "configs/yaml2stream.h"

#include "services/groupservice.h"
#include "services/productservice.h"
#include "services/apacheservice.h"
#include "services/mysqlservice.h"

#include "base/inforesult.h"

#include <QList>

class MainWindow;
class Service;

class Controller : public QObject
{
    Q_OBJECT
public:
    Controller();

    QString themeName();
    void setThemeName(QString themeName);

    QString langName();
    void setLangName(QString langName);

    QString logoPath();

    QString toolTitle();
    QString toolVersion();

    QString customStrParam(QString propPath);
    bool customBoolParam(QString propPath,bool defaultValue=true);

    QString mainProductCode();
    QString mainProductLaunch();

    QString applicationState() const;

    ProductService *mainProduct() const;
    QList<ProductService*> subProductList() const;
    QList<Service*> serviceList() const;
    Service *apacheService();
    QList<Service*> queryServiceList(bool includeNoService=false,bool includeIgnore=false);

    YamlStream *userConfig() const;

    void computeScale();
    void askOsArch();
    void loadLang(QString langName);

    void reStart(Service *service);
    void reStart();
    void oneSetup();
    void oneStop();
    void oneUninstall();

    void appendMsg(MsgResult msg);

    bool autoSelectPort(Service *service=nullptr);

    bool enableBackup();
    void executeBackup();

    void updateAppState(bool forcedQuery = true);
    void onAppStateChanged();

    bool isPreInstalled();

    void applyInitConfig();
    QList<MsgResult> basicsMsgList();

    Yaml2Stream *iniConfig() const;
    MainWindow* MainWin() { return m_mainWin; }
private:
    MainWindow *m_mainWin;

    QString m_applicationState;
    
    Yaml2Stream *m_iniConfig;
    YamlStream *m_userConfig;

    ProductService *m_mainProduct;
    QList<ProductService*> m_subProductList;
    QList<Service*> m_serviceList;
    
    void initInitConfig();
    void initUserConfig();
    void initLangConfig();

    void createMainProduct();
    void createSubProductList();
    void createSerivceList();
    void extractParamValues();
    
    bool checkApplicationPath();
    void setupServiceSignal();

private slots:
    void onReceiveMsg(QVariant msg);

    void onOneInstallFinished(bool result);
    void onOneStopFinished(bool result);
    void onOneUninstallFinished(bool result);
    void onStateChanged(QString);
    void onServiceStateChanged(QString);

public slots:
    void toActiveWindow(const QString&);
};

#endif // CONTROLLER_H
