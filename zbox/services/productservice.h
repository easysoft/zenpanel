#ifndef SUBOPER_H
#define SUBOPER_H

#include "base/service.h"
#include "configs/yaml2stream.h"

class Controller;

class ProductService : public Service
{
public:
    ProductService(Controller *controllor,Yaml2Stream *config,QString type);

    QString code();
    QString sourcePath();

    bool backupEnabled();
    QString backkupScript();
    QString backupPath();

    void extractParamValues();
};

#endif // SUBOPER_H
