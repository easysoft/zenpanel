/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef YAML2STREAM_H
#define YAML2STREAM_H

#include <QString>

#include "configs/yamlstream.h"

#include "yaml-cpp/yaml.h"
using namespace YAML;

class Yaml2Stream
{
public:
    Yaml2Stream(QString currentPath,QString defaultPath);
    Yaml2Stream(YAML::Node currentRoot, YAML::Node defaultRoot);
    Yaml2Stream(YAML::Node currentRoot);

    YAML::Node getNode(QString propPath);
    bool existNode(QString propPath);

    YamlStream* getCurrent();
    YamlStream* getDefault();

    QString getString(QString propPath,QString defaultValue="");
    bool getboolean(QString propPath,bool defaultValue=true);

    QList<int> getIntList(QString propPath);

private:
    YamlStream *m_currentStream;
    YamlStream *m_defaultStream;
};

#endif // YAML2STREAM_H
