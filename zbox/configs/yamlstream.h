#ifndef YAMLSTREAM_H
#define YAMLSTREAM_H

#include <QString>
#include "qyaml.h"


class YamlStream
{
public:
    YamlStream();
    YamlStream(QString filePath);
    YamlStream(YAML::Node node);

    YAML::Node getNode(QString propPath);
    YAML::Node getParentNode(QString propPath);
    bool existNode(QString propPath);

    QString getString(QString propPath,QString defaultValue="");
    bool getboolean(QString propPath,bool defaultValue=true);
    QList<int> getIntList(QString propPath);

    void setString(QString propPath,QString value);
    void setBoolean(QString propPath,bool value);
    void save();

private:
    QString m_filePath;
    YAML::Node m_rootNode;

    void load();
};

#endif // YAMLSTREAM_H
