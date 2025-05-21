/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "yaml2stream.h"

#include "utils/yamlutil.h"

#include <QDir>

Yaml2Stream::Yaml2Stream(QString currentPath, QString defaultPath)
{
    m_currentStream = new YamlStream(currentPath);
    m_defaultStream = new YamlStream(defaultPath);
}

Yaml2Stream::Yaml2Stream(Node currentRoot, Node defaultRoot)
{
    m_currentStream = new YamlStream(currentRoot);
    m_defaultStream = new YamlStream(defaultRoot);
}

Yaml2Stream::Yaml2Stream(Node currentRoot)
{
    m_currentStream = new YamlStream(currentRoot);
    m_defaultStream = new YamlStream();
}

Node Yaml2Stream::getNode(QString propPath)
{
    YAML::Node nowNode = m_currentStream->getNode(propPath);
    YAML::Node defaultNode = m_defaultStream->getNode(propPath);

    if(nowNode && nowNode.IsNull() == false)
        return nowNode;

    return defaultNode;
}

bool Yaml2Stream::existNode(QString propPath)
{
    bool nowFlag = m_currentStream->existNode(propPath);
    bool defaultFlag = m_defaultStream->existNode(propPath);

    bool flag = nowFlag || defaultFlag;

    return flag;
}

YamlStream* Yaml2Stream::getCurrent()
{
    return m_currentStream;
}

YamlStream* Yaml2Stream::getDefault()
{
    return m_defaultStream;
}

QString Yaml2Stream::getString(QString propPath, QString defaultValue)
{
    YAML::Node node = getNode(propPath);

    if(node)
    {
        std::string str = node.as<std::string>();
        return YAMLUtil::toQString(str);
    }

    return defaultValue;
}

bool Yaml2Stream::getboolean(QString propPath, bool defaultValue)
{
    YAML::Node node = getNode(propPath);

    if(node)
    {
        return node.as<bool>();
    }

    return defaultValue;
}

QList<int> Yaml2Stream::getIntList(QString propPath)
{
    QList<int> currentList = m_currentStream->getIntList(propPath);
    QList<int> defaultList = m_defaultStream->getIntList(propPath);

    if(currentList.length() > 0)
        return currentList;

    return defaultList;
}
