/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "envrunner.h"

#include "qyaml.h"
#include "utils/yamlutil.h"
#include "utils/envutil.h"
#include "base/gparams.h"
#include "utils/processutil.h"

#include <QProcess>
#include <QProcessEnvironment>
#include <QDir>


EnvRunner::EnvRunner(Yaml2Stream *iniConfig):Runner()
{
    this->m_initConfig = iniConfig;
}

void EnvRunner::run()
{
    YAML::Node envPathNode = m_initConfig->getNode("common.envPath");
    QList<YAML::Node> nodeList = YAMLUtil::getList(envPathNode);

    for(int i=0;i<nodeList.length();i++)
    {
        YAML::Node node = nodeList[i];

        std::string stdConfigPathStr = node.as<std::string>();
        QString configPathStr = YAMLUtil::toQString(stdConfigPathStr);

        QString pathStr = EnvUtil::replaceText(configPathStr);

        pathStr = QDir::toNativeSeparators(pathStr);
        addPath(pathStr,i);

        msleep(200);
    }
}

void EnvRunner::addPath(QString path,int index)
{
    QString tmpPath = EnvUtil::getPath("zbox/add_path.bat");
    QString targetPath = EnvUtil::getPath("tmp/env/p" + QString::number(index) + ".bat");

    GParams::instance()->addParam("ENV_PATH",path);
    EnvUtil::replaceConfig(tmpPath,targetPath);

    ProcessUtil::excuteCommand(targetPath);
}
