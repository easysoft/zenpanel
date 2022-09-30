#ifndef VBOXSERVICE_H
#define VBOXSERVICE_H

#include "base/service.h"

#include <Windows.h>

class Controller;
class QuickOnService : public Service
{
public:
    QuickOnService(Controller *controllor, Yaml2Stream *config, QString type);
    virtual ~QuickOnService();

    static void PrepareCMD();
    static int ExecCmd(char read_buffer[], const char* fmt, ...);

    virtual QString queryState() override;
protected:
    virtual bool installServiceImpl(SendProxy *proxy) override;
    virtual bool uninstallServiceImpl(SendProxy *proxy) override;
    virtual bool startServiceImpl(SendProxy *proxy) override;
    virtual bool stopServiceImpl(SendProxy *proxy) override;
    virtual bool killServiceImpl(SendProxy *proxy) override;
    virtual bool restartServiceImpl(SendProxy *proxy) override;
    virtual bool lazyInstallServiceImpl(SendProxy *proxy) override;

private:
    int IsInstalled();
    int RunVirtualBoxMSI(char* buf);
    int IsOvaExist(char* buf);
    int ImportOVA(char* buf);
    int RemoveOVA(char* buf);

    void VBoxManageFullPath();

private:
    bool m_bIsExist;

    SC_HANDLE m_hScHandle, m_hScHandleVbox;
}; // VBoxService

#endif // VBOXSERVICE_H
