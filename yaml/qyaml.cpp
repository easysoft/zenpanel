#include "qyaml.h"

#include <QFile>
#include <QFileInfo>
#include <QDebug>


QYaml::QYaml()
{

}

YAML::Node QYaml::load(const QString &fileName)
{
    return  YAML::LoadFile(fileName.toStdString());
}



