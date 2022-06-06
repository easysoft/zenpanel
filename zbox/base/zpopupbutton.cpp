#include "zpopupbutton.h"

#include <QApplication>
#include <QStyle>
#include <QLabel>

#include "base/zpopupmenu.h"
#include "base/gbus.h"

ZPopupButton::ZPopupButton(ZPopupMenu *parent):QPushButton(nullptr)
{
    m_menu = parent;
    m_check = false;
    connect(this,SIGNAL(clicked()),this,SLOT(onClicked()));

    m_checkLbl = new QLabel(this);
    m_checkLbl->setProperty("forUse","checked");
    m_checkLbl->setText("✔");

    m_checkLbl->adjustSize();
    m_checkLbl->move(ts(3),ts(5));

    setCheck(false);
}

void ZPopupButton::setPopup(ZPopupMenu *widget)
{
    style()->unpolish(widget);

    this->m_popWidget = widget;
    this->m_popWidget->setIsNested(true);
    this->m_popWidget->setProperty("forRadius","nestedMenu");
    m_popWidget->setVisible(false);

    style()->polish(widget);
}

void ZPopupButton::closePopup()
{
    if(m_popWidget != nullptr)
    {
        this->m_popWidget->setVisible(false);
    }
}

void ZPopupButton::adjustAfterLang()
{
    if(m_popWidget != nullptr)
    {
        this->m_popWidget->adjustSize();
    }

    adjustSize();
}

bool ZPopupButton::hasPopup()
{
    return m_popWidget == nullptr ? false : true;
}

bool ZPopupButton::popContainCursor()
{
    if(m_popWidget == nullptr || m_popWidget->isVisible() == false)
        return false;

    QPoint gPoint = QCursor::pos();
    QPoint nowPoint = m_popWidget->mapFromGlobal(gPoint);
    QSize nowSize = m_popWidget->size();
    QRect nowRect = QRect(0,0,nowSize.width(),nowSize.height());
    if(nowRect.contains(nowPoint))
    {
        return true;
    }

    return false;
}

bool ZPopupButton::check() const
{
    return m_check;
}

void ZPopupButton::setCheck(bool checkValue)
{
    m_check = checkValue;

    m_checkLbl->setVisible(checkValue);
}

void ZPopupButton::enterEvent(QEvent *event)
{
    m_menu->closeAllPopups();
    
    if(m_popWidget != nullptr)
    {
        QPoint selfPosition = QPoint(0,0);
        selfPosition = this->mapToGlobal(selfPosition);
        selfPosition = QApplication::activeWindow()->mapFromGlobal(selfPosition);

        QPoint popupPosition = QPoint(selfPosition.x() + this->size().width() + 1,selfPosition.y());
        m_popWidget->move(popupPosition);
        this->m_popWidget->setVisible(true);
    }


    QPushButton::enterEvent(event);
}

void ZPopupButton::onClicked()
{
    if(hasPopup() == false)
        GBus::instance()->onCloseMenu();
}
