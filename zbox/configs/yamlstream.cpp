#include "yamlstream.h"

#include <QFileInfo>
#include <QDir>
#include <iostream>
#include <fstream>

#include "utils/yamlutil.h"

using namespace std;

YamlStream::YamlStream()
{

}

YamlStream::YamlStream(QString filePath)
{
    this->m_filePath = filePath;
    load();
}

YamlStream::YamlStream(YAML::Node node)
{
    m_rootNode = node;
}

YAML::Node YamlStream::getNode(QString propPath)
{
    if(!m_rootNode || m_rootNode.IsNull() == true)
    {
        YAML::Node node;
        YAML::Node undefinedNode = node[YAMLUtil::toStdString("nulltest")];

        return  undefinedNode;
    }

    QStringList propsList = propPath.split('.');
    YAML::Node parentNode = YAML::Clone(m_rootNode);
    for(int i=0;i<propsList.length();i++)
    {
        std::string propName = YAMLUtil::toStdString(propsList[i]);
        parentNode = parentNode[propName];

        if(!parentNode)
            return parentNode;
    }

    return parentNode;
}

YAML::Node YamlStream::getParentNode(QString propPath)
{
    if(!m_rootNode || m_rootNode.IsNull() == true)
    {
        YAML::Node node;
        YAML::Node undefinedNode = node[YAMLUtil::toStdString("nulltest")];

        return  undefinedNode;
    }

    QStringList propsList = propPath.split('.');
    YAML::Node parentNode = YAML::Clone(m_rootNode);
    for(int i=0;i<propsList.length()-1;i++)
    {
        std::string propName = YAMLUtil::toStdString(propsList[i]);
        parentNode = parentNode[propName];

        if(!parentNode)
            return parentNode;
    }

    return parentNode;
}

bool YamlStream::existNode(QString propPath)
{
    if(!m_rootNode || m_rootNode.IsNull() == true)
    {
        return false;
    }

    QStringList propsList = propPath.split('.');
    YAML::Node parentNode = YAML::Clone(m_rootNode);
    for(int i=0;i<propsList.length();i++)
    {
        std::string propName = YAMLUtil::toStdString(propsList[i]);
        parentNode = parentNode[propName];

        if(!parentNode)
            return false;
    }

    return parentNode ? true : false;
}

QString YamlStream::getString(QString propPath, QString defaultValue)
{
    YAML::Node node = getNode(propPath);

    if(node && node.IsNull() == false && node.IsScalar())
    {
        std::string str = node.as<std::string>();
        return YAMLUtil::toQString(str);
    }

    return defaultValue;
}

bool YamlStream::getboolean(QString propPath, bool defaultValue)
{
    YAML::Node node = getNode(propPath);

    if(node && node.IsNull() == false && node.IsScalar())
    {
        return  node.as<bool>();
    }

    return defaultValue;
}

QList<int> YamlStream::getIntList(QString propPath)
{
    YAML::Node node = getNode(propPath);

    QList<int> results;

    if(node && node.IsNull() == false && node.IsSequence())
    {
        int defaultCount = node.size();
        for(int i=0;i<defaultCount;i++)
        {
            YAML::Node itemNode = node[i];
            int stdPort = itemNode.as<int>();
            QString port = QString::number(stdPort);

            results.append(port.toUInt());
        }
    }

    return results;
}

void YamlStream::setString(QString propPath, QString value)
{
    std::string stdValue = value.toStdString();

    QStringList propList = propPath.split(".");
    if(propList.length() == 1)
    {
        std::string stdFirstName = propList[0].toStdString();
        m_rootNode[stdFirstName] = stdValue;
    }
    else if(propList.length() > 1)
    {
        std::string stdFirstName = propList[0].toStdString();
        YAML::Node itemNode = m_rootNode[stdFirstName];
        for(int i=1;i<propList.length()-1;i++)
        {
            std::string stdPropName = propList[i].toStdString();
            itemNode = itemNode[stdPropName];
        }
        std::string stdLastName = propList[propList.length()-1].toStdString();
        itemNode[stdLastName] = stdValue;
    }
}

void YamlStream::setBoolean(QString propPath, bool value)
{
    bool stdValue = value;

    QStringList propList = propPath.split(".");
    if(propList.length() == 1)
    {
        std::string stdFirstName = propList[0].toStdString();
        m_rootNode[stdFirstName] = stdValue;
    }
    else if(propList.length() > 1)
    {
        std::string stdFirstName = propList[0].toStdString();
        YAML::Node itemNode = m_rootNode[stdFirstName];
        for(int i=1;i<propList.length()-1;i++)
        {
            std::string stdPropName = propList[i].toStdString();
            itemNode = itemNode[stdPropName];
        }
        std::string stdLastName = propList[propList.length()-1].toStdString();
        itemNode[stdLastName] = stdValue;
    }
}

void YamlStream::save()
{
    std::string stdFilePath = m_filePath.toStdString();

    ofstream fout(stdFilePath);
    fout << m_rootNode;
    fout.close();
}

void YamlStream::load()
{
    QFileInfo fileInfo(m_filePath);
    if(fileInfo.isFile() == false)
        return;

    QYaml doc;
    m_rootNode = doc.load(m_filePath);
}
