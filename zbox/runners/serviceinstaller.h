/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef SINGLEINSTALLER_H
#define SINGLEINSTALLER_H

#include "base/runner.h"
#include "base/service.h"

class ServiceInstaller : public Runner
{
    Q_OBJECT
public:
    ServiceInstaller(Service* service);

    void installSingleService();
    void reinstallSingleService();
    void stopSingleSerivce();
    void restartSingleService();
    void uninstallSingleService();

private:
    Service *m_service;

    QString m_execute;

    void installSingleServiceImpl();
    void reInstallSingleServiceImpl();
    void stopSingleSerivceImpl();
    void restartSingleServiceImpl();
    void uninstallSingleServiceImpl();

protected:
    void run();
};

#endif // SINGLEINSTALLER_H
