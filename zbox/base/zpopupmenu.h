#ifndef ZPOPUPMENU_H
#define ZPOPUPMENU_H

#include <QWidget>
#include <QVBoxLayout>

#include "base/zpopupbutton.h"
#include "base/gscale.h"

class ZPopupMenu : public QWidget,GScale
{
    Q_OBJECT
public:
    ZPopupMenu(QWidget *parent=nullptr);

    void add(ZPopupButton *pupupBtn);
    void closeAllPopups();

    void adjustAfterLang();

    void attachWidget(QWidget *widget);

    bool isNested() const;
    void setIsNested(bool isNested);

private:
    QVBoxLayout *m_layout;
    QList<ZPopupButton*> m_menuList;

    bool m_isNested;
    QWidget *m_attach=nullptr;

private slots:
    void closeMenu();
    void forceCloseMenu();
};

#endif // ZPOPUPMENU_H
