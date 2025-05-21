/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
