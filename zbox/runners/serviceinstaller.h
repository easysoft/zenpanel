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
