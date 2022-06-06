#include "productservice.h"

#include "utils/envutil.h"
#include "controller.h"
#include "base/gparams.h"

ProductService::ProductService(Controller *controllor,Yaml2Stream *config,QString type):
    Service(controllor,config,type)
{
}

QString ProductService::code()
{
    return m_config->getString("code");
}

QString ProductService::sourcePath()
{
    QString path = m_config->getString("path",type() + "/");
    return EnvUtil::getPath("app/" + path);
}

bool ProductService::backupEnabled()
{
    return  m_config->getboolean("backup.enabled");
}

QString ProductService::backkupScript()
{
    return  m_config->getString("backup.script");
}

QString ProductService::backupPath()
{
    return  m_config->getString("backup.path");
}

void ProductService::extractParamValues()
{
    QString key = code().toUpper();

    GParams::instance()->addParam(key + "_PATH", sourcePath());
}
