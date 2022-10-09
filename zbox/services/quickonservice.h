#ifndef VBOXSERVICE_H
#define VBOXSERVICE_H

#include "base/service.h"

#include <Windows.h>

#include <string>
#include <memory>
#include <functional>

/*
code	int	是	
message	string	是	
timestamp	string	是	
data	object	是	
  ∟  tls_cert_path	string	是	
  ∟  tls_key_path	string	是	
  ∟  domain	string	是	域名
  ∟  k8s-tls	string	是	k8s tls yaml
*/
struct quickon_record
{
    std::string tls_cert_path;
    std::string tls_key_path;
    std::string domain;
    std::string k8s_tls;

    std::string message;
};
class Controller;
class QuickOnService : public QObject, public Service
{
    Q_OBJECT
public:
    QuickOnService(Controller *controllor, Yaml2Stream *config, QString type);
    virtual ~QuickOnService();

    bool QueryUrl(std::shared_ptr<std::string> domain, std::string& message);
    bool SignUrl(quickon_record& record);

    void QueryPortRnd(int& http_port, int& https_port);
    void SaveInitEnv(std::shared_ptr<std::string> domain, int http_port, int https_port);

    static void PrepareCMD();
    static int ExecCmd(char read_buffer[], Service *service, SendProxy *proxy, const char* fmt, ...);

    virtual QString queryState() override;

signals:
    void HttpPostData(std::shared_ptr<std::string> url, std::shared_ptr<std::string> data, std::shared_ptr<std::string> reply);
    void NotifyQuickOnInfo(const std::shared_ptr<std::string> domain, int http_port, int https_port);
    
public:
    virtual void SetupSignal() override;

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

    bool QueryUrlLocal(std::shared_ptr<std::string> domain);
    bool QueryUrlNet(std::shared_ptr<std::string> domain, std::string& message);
    void QueryPortLocal(int& http_port, int& https_port);

private:
    bool m_bIsExist;

    SC_HANDLE m_hScHandle, m_hScHandleVbox;
}; // VBoxService

#endif // VBOXSERVICE_H
