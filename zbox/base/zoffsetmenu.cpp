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
