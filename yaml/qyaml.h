/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef QYAML_H
#define QYAML_H

#include <QString>

#include "yaml-cpp/yaml.h"

class QYaml
{
public:
    QYaml();

    YAML::Node load(const QString &fileName);
};

#endif // QYAML_H
