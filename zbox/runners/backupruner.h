/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
