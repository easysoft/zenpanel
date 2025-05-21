/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef SUBOPER_H
#define SUBOPER_H

#include "base/service.h"
#include "configs/yaml2stream.h"

class Controller;

class ProductService : public Service
{
public:
    ProductService(Controller *controllor,Yaml2Stream *config,QString type);

    QString code();
    QString sourcePath();

    bool backupEnabled();
    QString backkupScript();
    QString backupPath();

    void extractParamValues();
};

#endif // SUBOPER_H
