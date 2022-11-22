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

typedef void (*PostDataCallback)(const std::string&);

class Controller;
class QuickOnService : public QObject, public Service
{
    Q_OBJECT
public:
    QuickOnService(Controller *controllor, Yaml2Stream *config, QString type);
    virtual ~QuickOnService();

    void ReBindHttpPost(const std::function<void(std::shared_ptr<std::string>, std::shared_ptr<std::string>)>& cb);

    bool IsLocalConfigExist();

    void QueryUrl(const std::string& domain, const std::string& sub, std::function<void(bool, const std::string&)> cb);
    void SignUrl();

    void QueryPortRnd(int& http_port, int& https_port);
    void SaveInitEnv(std::shared_ptr<std::string> domain, int http_port, int https_port);
    int IsInstalled();

    static void PrepareCMD();
    static int ExecCmd(char read_buffer[], Service *service, SendProxy *proxy, const char* fmt, ...);
//    virtual QString queryState() override;

signals:
    void HttpPostData(std::shared_ptr<std::string> url, std::shared_ptr<std::string> data);
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
    int RunVirtualBoxMSI(char* buf);
    int IsOvaExist(char* buf);
    int ImportOVA(char* buf);
    int RemoveOVA(char* buf);

    void VBoxManageFullPath();

    bool QueryUrlLocal();
    bool QueryUrlNet(const std::string& domain, const std::string& sub, std::function<void(bool, const std::string&)> cb);
    void QueryPortLocal(int& http_port, int& https_port);

private:
    bool m_bIsExist;
    std::string m_Domain;
    SC_HANDLE m_hScHandle, m_hScHandleVbox;
}; // VBoxService

#endif // VBOXSERVICE_H
