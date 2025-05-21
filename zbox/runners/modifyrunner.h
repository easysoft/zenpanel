/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef MODIFYRUNNER_H
#define MODIFYRUNNER_H

#include "base/runner.h"
#include "base/servicebase.h"
#include "services/productservice.h"
#include "configs/yamlstream.h"

class ModifyRunner : public Runner
{
    Q_OBJECT
public:
    ModifyRunner(ProductService *mainOper,QList<ProductService*> subList,QList<Service*> serviceList,Service *service,YamlStream *userConfig);
    ModifyRunner(ProductService *mainOper,QList<ProductService*> subList,QList<Service*> serviceList);

    void startConfig();
    void startPort(QString port);
    void startUser(QString newAccount,QString newPassowrd);

    bool replaceConfigAll();

private:
    QString m_excute;

    QString m_newPort;
    QString m_newAccount;
    QString m_newPassword;

    YamlStream *m_userConfig;

    Service *m_service;
    ProductService *m_mainOper;
    QList<ProductService*> m_subList;
    QList<Service*> m_servieList;



    void startConfigImpl();
    void startPortImpl();
    void startUserImpl();

protected:
    void run();

signals:
    void modifySccess(QString);
};

#endif // MODIFYRUNNER_H
