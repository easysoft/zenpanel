#ifndef INSTALLER_H
#define INSTALLER_H

#include <QVariant>
#include <QThread>

#include "base/sendproxy.h"
#include "base/servicebase.h"

class Service : public ServiceBase
{
public:
    Service(Controller *controllor,Yaml2Stream *config,QString type);

    bool installConfig(ConfigInfo info,SendProxy *proxy);
    virtual bool installConfig(bool ignoreBug,SendProxy *proxy);
    bool excCommand(CommandInfo info,SendProxy *proxy);
    virtual bool excCommand(bool ignoreBug,SendProxy *proxy);

    bool installService(SendProxy *proxy);
    bool uninstallService(SendProxy *proxy);
    bool startService(SendProxy *proxy);
    bool stopService(SendProxy *proxy);
    bool killService(SendProxy *proxy);
    bool restartService(SendProxy *proxy);
    bool lazyInstallService(SendProxy *proxy);

    MsgResult getInfoMsg(QString msg,QString placeholder1="",QString placeholder2="",QString directOutput="");
    MsgResult getSuccessMsg(QString msg="",QString placeholder1="",QString placeholder2="",QString directOutput="");
    MsgResult getErrorMsg(QString msg,QString placeholder1="",QString placeholder2="",QString directOutput="");

protected:
    virtual bool installServiceImpl(SendProxy *proxy);
    virtual bool uninstallServiceImpl(SendProxy *proxy);
    virtual bool startServiceImpl(SendProxy *proxy);
    virtual bool stopServiceImpl(SendProxy *proxy);
    virtual bool killServiceImpl(SendProxy *proxy);
    virtual bool restartServiceImpl(SendProxy *proxy);
    virtual bool lazyInstallServiceImpl(SendProxy *proxy);

    void sleepBetween();
};

#endif // INSTALLER_H
