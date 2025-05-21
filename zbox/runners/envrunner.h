/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef ENVPATHRUNNER_H
#define ENVPATHRUNNER_H

#include "base/runner.h"
#include "configs/yaml2stream.h"

class EnvRunner : public Runner
{
public:
    EnvRunner(Yaml2Stream *iniConfig);

private:
    Yaml2Stream *m_initConfig;

    void addPath(QString path,int index);

protected:
    void run();
};

#endif // ENVPATHRUNNER_H
