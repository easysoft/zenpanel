#ifndef APACHESERVICEOPER_H
#define APACHESERVICEOPER_H

#include "base/service.h"

class ApacheService : public Service
{
public:
    ApacheService(Controller *controllor,Yaml2Stream *config,QString type);

    QString versionCommand();
    void extractParamValues();    

    InfoResult changePort(QString newPort);
    InfoResult changeUser(QString newAccount, QString newPassword);

protected:
    virtual bool installServiceImpl(SendProxy *proxy) override;
    virtual bool uninstallServiceImpl(SendProxy *proxy) override;
    virtual bool startServiceImpl(SendProxy *proxy) override;
    virtual bool stopServiceImpl(SendProxy *proxy) override;
    virtual bool killServiceImpl(SendProxy *proxy) override;
    virtual bool restartServiceImpl(SendProxy *proxy) override;
    virtual bool lazyInstallServiceImpl(SendProxy *proxy) override;
};

#endif // APACHESERVICEOPER_H
