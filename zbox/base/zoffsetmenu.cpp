/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "zoffsetmenu.h"

ZOffsetMenu::ZOffsetMenu(QPushButton *refer, QWidget *parent) : QMenu(parent),
    m_setOffset(false),
    m_referBtn(refer)

{

}

void ZOffsetMenu::setOffset(int x, int y)
{
    m_setOffset = true;
    m_offset = QPoint(x,y);
}

void ZOffsetMenu::showEvent(QShowEvent *event)
{
    if(m_setOffset == true)
    {
        QPoint p = this->pos();
        this->move(p.x() + m_offset.x(), p.y() + m_offset.y());
        //this->move(p.x(), p.y());
    }
    else if(m_referBtn != nullptr)
    {
        QPoint p = this->pos();
        QRect geo = m_referBtn->geometry();
        this->move(p.x()+geo.width(), p.y() - geo.height());
    }
    else
    {
        QMenu::showEvent(event);
    }
}
