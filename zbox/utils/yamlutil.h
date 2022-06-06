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
