/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "servicecontainer.h"
#include "utils/constutil.h"

ServiceContainer::ServiceContainer(Controller *ctr,QWidget *parent) : QWidget(parent)
{
    this->m_ctr = ctr;

    createUI();
}

void ServiceContainer::adjustAfterLangChanged()
{
    assignUI();

    foreach(ServiceControl *control,m_controls)
    {
        control->adjustAfterLangChanged();
    }
}

void ServiceContainer::stateChanged(QString typeAndState)
{
    QStringList strList = typeAndState.split("|");

    QString type = strList[0];
    QString state = strList[1];

    foreach(ServiceControl *control,m_controls)
    {
        if(control->type() == type)
        {
            control->stateChanged(state);
            return;
        }
    }
}

void ServiceContainer::assignUI()
{
    QList<Service*> serviceList = m_ctr->queryServiceList(false,true);

    QSize mainSize = parentWidget()->size();

    int gap = ts(ConstUtil::GRID_GAP);

    int itemWidth = ConstUtil::itemWidth(mainSize.height(),gap);
    int itemHeight = ConstUtil::itemHeight(mainSize.height(),gap);

    int count = serviceList.count();
    QList<int> colList = QList<int>()<<0<<1<<2<<3<<2<<3<<3;

    int colMax = count > 6 ? 3 : colList[count];

    int startX = (mainSize.height() - (colMax * itemWidth + (colMax-1)*gap))/2;
    int startY = mainSize.height()/2 - itemHeight * 0.26;

    int x = 0;
    int y = 0;
    for(int i=0;i<colMax;i++)
    {

        ServiceControl *control = m_controls[i];

        control->move(x,y);
        control->setMinimumSize(itemWidth,itemHeight);
        control->setMaximumSize(itemWidth,itemHeight);

        x += itemWidth + gap;
    }

    x = 0;
    y = 0 + itemHeight + gap;
    for(int i=colMax;i<count;i++)
    {
        ServiceControl *control = m_controls[i];

        control->move(x,y);
        control->setMinimumSize(itemWidth,itemHeight);
        control->setMaximumSize(itemWidth,itemHeight);

        x += itemWidth + gap;
    }

    this->move(startX,startY);
    this->adjustSize();
}

void ServiceContainer::createUI()
{
    QList<Service*> serviceList = m_ctr->queryServiceList(false,true);

    for(int i=0;i<serviceList.length();i++)
    {
        ServiceControl *control = new ServiceControl(m_ctr,serviceList[i],this);

        control->init();
        m_controls.append(control);
        control->show();
    }
}

