/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
