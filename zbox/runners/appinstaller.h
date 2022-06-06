#ifndef ONEINSTALLER_H
#define ONEINSTALLER_H

#include "base/runner.h"
#include "services/productservice.h"

#include "base/service.h"

class AppInstaller : public Runner
{
    Q_OBJECT
public:
    explicit AppInstaller(ProductService *mainProduct,QList<ProductService*> *subProductList,QList<Service*> *serviceList);

    void startApplication();
    void stopApplication();
    void uninstallApplication();
    void reStartApplication();

private:
    ProductService *m_mainProduct;
    QList<ProductService*> *m_subProductList;
    QList<Service*> *m_servieList;

    QString m_execute;

    void startApplicationImpl();
    void stopApplicationImpl();
    void uninstallApplicationImpl();
    void reStartApplicationImpl();

    bool isUnifiedState(QString state);

protected:
    void run();
};

#endif // ONEINSTALLER_H
