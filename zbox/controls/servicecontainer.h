#ifndef SERVICECONTAINER_H
#define SERVICECONTAINER_H

#include <QWidget>

#include "controller.h"
#include "controls/servicecontrol.h"
#include "base/gscale.h"
#include "base/glang.h"

class ServiceContainer : public QWidget,GScale,GLang
{
    Q_OBJECT
public:
    explicit ServiceContainer(Controller *ctr,QWidget *parent = nullptr);

    void adjustAfterLangChanged();
    void stateChanged(QString typeAndState);

private:
    Controller *m_ctr;
    QList<ServiceControl*> m_controls;

    void assignUI();
    void createUI();

signals:

};

#endif // SERVICECONTAINER_H
