#ifndef MYSQLSERVICEOPER_H
#define MYSQLSERVICEOPER_H

#include "base/service.h"

class MysqlService : public Service
{
public:
    MysqlService(Controller *controllor,Yaml2Stream *config,QString type);

    QString account();
    QString password();

    bool canChangeAccount();
    void extractParamValues();

    InfoResult changePort(QString newPort);
    InfoResult changeUser(QString newAccount, QString newPassword);
};

#endif // MYSQLSERVICEOPER_H
