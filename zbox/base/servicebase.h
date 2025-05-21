/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef SERVICEBASE_H
#define SERVICEBASE_H

#include <QString>
#include <QList>

#include "qyaml.h"
#include "base/infoconfig.h"
#include "configs/yaml2stream.h"
#include "base/inforesult.h"

class Controller;

class ServiceBase
{
public:
    ServiceBase(Controller *controllor,Yaml2Stream *config,QString type);

    QString type();
    QString title();
    QString version();
    virtual QString sourcePath();

    virtual QString serviceName();
    QString exePath();
    QString exeName();
    virtual QString versionCommand();

    QString launch();
    QString log();

    QString port();
    QString initPort();
    QList<int> autoPorts();
    bool fixPort();

    QString vcRumtime();

    bool ignore();

    virtual QString account();
    virtual QString password();

    QString state();
    void setState(QString &state);

    bool canChangePort();
    virtual InfoResult changePort(QString newPort);

    bool enableAuth();
    bool needMakeAuth();
    virtual bool canChangeAccount();
    bool canChangeUser();
    virtual InfoResult changeUser(QString newAccount,QString newPassword);

    virtual bool isFamilyType(QString type);
    
    QList<ConfigInfo> configList();
    QList<CommandInfo> commandList();

    QString getDefaultConfigTplPath(QString selfPath);
    QString getDefaultConfigPath(QString selfPath);

    bool checkVersion();
    bool checkSourcePath();
    QString initializeCheck();

    void openBrowser();
    void openSubUrl(const QString& url);
    void openLog();

    virtual void refreshState();
    virtual bool isWindowService();
    virtual bool modifyUser();
    virtual void extractParamValues();

    virtual QString queryState();

    ServiceBase *getParent() const;
    void setParent(ServiceBase *parent);

protected:
    QString m_type;
    QString m_state;

    ServiceBase *m_parent;

    Controller *m_ctr;
    Yaml2Stream *m_config;

    QList<ConfigInfo> m_configList;
    QList<CommandInfo> m_commandList;

    void initConfigList();
    void initCommandList();

    QString toCase(QString title);
};

#endif // SERVICEBASE_H
