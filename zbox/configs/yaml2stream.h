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
