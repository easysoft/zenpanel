#include "yamlutil.h"

std::string YAMLUtil::toStdString(QString str)
{
    //return str.toStdString();
    return std::string((const char *)str.toLocal8Bit().constData());
}

QString YAMLUtil::toQString(std::string str)
{
    return QString::fromStdString(str);
}

QList<QString> YAMLUtil::mapKeys(YAML::Node node)
{
    QList<QString> keys;

    if(node.Type() != YAML::NodeType::Map)
        return keys;

    for(const auto& it : node )
    {
        std::string key = it.first.as<std::string>();

        keys.append(toQString(key));
    }

    return keys;
}

QList<YAML::Node> YAMLUtil::getList(YAML::Node node)
{
    QList<YAML::Node> results;

    if(!node || node.IsNull())
        return results;

    if(node.Type() == YAML::NodeType::Sequence)
    {
        int size = node.size();
        for(int i=0;i<size;i++)
            results.append(node[i]);
    }
    else
    {
        results.append(node);
    }

    return results;
}
