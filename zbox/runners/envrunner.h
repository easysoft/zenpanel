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
