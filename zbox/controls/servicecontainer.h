/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
