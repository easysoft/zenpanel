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
