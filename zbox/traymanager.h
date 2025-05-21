/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef TRAYMANAGER_H
#define TRAYMANAGER_H

#include <QWidget>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QObject>

#include "base/glang.h"

class Controller;

class TrayManager: public QObject,GLang
{
    Q_OBJECT
public:
    explicit TrayManager(QWidget *mainWidget,Controller *ctr);

    void start();
    void updateIcon();
    void adjustAfterLang();

private:
    Controller *m_ctr;

    QWidget *m_mainWidget;

    QSystemTrayIcon *m_tray;

    QMenu *m_menu;
    QAction *m_quitAction;
    QAction *m_restoryAction;

private slots:
    void quit();
    void restory();
    void trayIconAction(QSystemTrayIcon::ActivationReason reason);
};

#endif // TRAYMANAGER_H
