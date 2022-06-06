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
