#ifndef APACHESERVICEOPER_H
#define APACHESERVICEOPER_H

#include "base/service.h"

class ApacheService : public Service
{
public:
    ApacheService(Controller *controllor,Yaml2Stream *config,QString type);

    QString versionCommand();
    void extractParamValues();    

    InfoResult changePort(QString newPort);
    InfoResult changeUser(QString newAccount, QString newPassword);
};

#endif // APACHESERVICEOPER_H
