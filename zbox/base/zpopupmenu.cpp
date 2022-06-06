#include "zpopupmenu.h"

#include <QVariant>

#include "base/gbus.h"

ZPopupMenu::ZPopupMenu(QWidget *parent):QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    m_isNested = false;

    setProperty("forUse","customMenu");
    setProperty("forRadius","mainMenu");
    //setAttribute(Qt::WA_TranslucentBackground);
    //setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    m_layout = new QVBoxLayout();
    m_layout->setContentsMargins(0,ts(4),0,ts(4));
    m_layout->setSpacing(0);
    this->setLayout(m_layout);

    connect(GBus::instance(), SIGNAL(pressed()), this, SLOT(closeMenu()));
    connect(GBus::instance(), SIGNAL(closeMenu()), this, SLOT(forceCloseMenu()));
}

void ZPopupMenu::add(ZPopupButton *pupupBtn)
{
    pupupBtn->adjustSize();
    m_layout->addWidget(pupupBtn);
    m_menuList.append(pupupBtn);
}

void ZPopupMenu::closeAllPopups()
{
    foreach(ZPopupButton *btn,m_menuList)
    {
        btn->closePopup();
    }
}

void ZPopupMenu::adjustAfterLang()
{
    foreach(ZPopupButton *btn,m_menuList)
    {
        btn->adjustAfterLang();
    }

    adjustSize();
}

void ZPopupMenu::attachWidget(QWidget *widget)
{
    m_attach = widget;
}

bool ZPopupMenu::isNested() const
{
    return m_isNested;
}

void ZPopupMenu::setIsNested(bool isNested)
{
    m_isNested = isNested;
}

void ZPopupMenu::closeMenu()
{
    if(m_isNested == true || this->isVisible() == false)
        return;

    QPoint gPoint = QCursor::pos();
    QPoint nowPoint = this->mapFromGlobal(gPoint);
    QSize nowSize = this->size();
    QRect nowRect = QRect(0,0,nowSize.width(),nowSize.height());
    if(nowRect.contains(nowPoint))
    {
        return;
    }

    foreach(ZPopupButton *zBtn,m_menuList)
    {
        if(zBtn->popContainCursor() == true)
            return;
    }

    closeAllPopups();
    setVisible(false);

    if(m_attach != nullptr)
        m_attach->setVisible(false);
}

void ZPopupMenu::forceCloseMenu()
{
    if(m_isNested == true)
        return;

    closeAllPopups();
    setVisible(false);

    if(m_attach != nullptr)
        m_attach->setVisible(false);
}
