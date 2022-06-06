#ifndef INFOCONFIG_H
#define INFOCONFIG_H

#include <QString>

struct ConfigInfo
{
    QString fromPath;
    QString toPath;
};

struct CommandInfo
{
    QString type;
    QString content;
};

#endif // INFOCONFIG_H
