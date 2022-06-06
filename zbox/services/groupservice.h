#ifndef GROUPSERVICE_H
#define GROUPSERVICE_H

#include "base/service.h"
#include "configs/yaml2stream.h"

class Controller;

class GroupService : public Service
{
public:
   GroupService(Controller *controllor,Yaml2Stream *config,QString type);

   bool isFamilyType(QString type);

   QString serviceName();
   void refreshState();
   bool isWindowService();

   QString queryState();

   bool installConfig(bool ignoreBug, SendProxy *proxy);
   bool excCommand(bool ignoreBug, SendProxy *proxy);

   bool installServiceImpl(SendProxy *proxy);
   bool uninstallServiceImpl(SendProxy *proxy);
   bool startServiceImpl(SendProxy *proxy);
   bool stopServiceImpl(SendProxy *proxy);
   bool killServiceImpl(SendProxy *proxy);
   bool restartServiceImpl(SendProxy *proxy);
   bool lazyInstallServiceImpl(SendProxy *proxy);

   void extractParamValues();

private:
    QList<Service*> m_members;

    void createMembers();

};

#endif // GROUPSERVICE_H
