#ifndef BACKUPRUNER_H
#define BACKUPRUNER_H

#include <QVariant>

#include "base/runner.h"
#include "services/productservice.h"
#include "base/infoconfig.h"

class BackupRuner : public Runner
{
    Q_OBJECT
public:
    BackupRuner(ProductService *product);

private:
    ProductService *m_product;

protected:
    void run();
};

#endif // BACKUPRUNER_H
