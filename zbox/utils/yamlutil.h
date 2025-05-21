/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef YAMLUTIL_H
#define YAMLUTIL_H

#include "qyaml.h"

#include <QList>

class YAMLUtil
{
public:
    static std::string toStdString(QString str);
    static QString toQString(std::string str);
    static QList<QString> mapKeys(YAML::Node node);
    static QList<YAML::Node> getList(YAML::Node node);
};

#endif // YAMLUTIL_H
