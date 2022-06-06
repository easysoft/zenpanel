#include "traymanager.h"

#include <QApplication>

#include "utils/envutil.h"
#include "controller.h"

TrayManager::TrayManager(QWidget *mainWidget,Controller *ctr)
{
    this->m_ctr = ctr;
    this->m_mainWidget = mainWidget;
}

void TrayManager::start()
{
    m_tray = new QSystemTrayIcon(this->m_mainWidget);
    m_tray->setIcon(QIcon(m_ctr->logoPath()));
    connect(m_tray,SIGNAL(activated(QSystemTrayIcon::ActivationReason)),this,SLOT(trayIconAction(QSystemTrayIcon::ActivationReason)));

    m_menu = new QMenu(this->m_mainWidget);
    m_menu->setProperty("forUse","tray");
    m_restoryAction = new QAction(this);
    //m_restoryAction->setIcon(QIcon(EnvUtil::resPath("zentao.ico")));
    m_restoryAction->setText(tlng("tray.show"));
    m_quitAction = new QAction(this);
    //m_quitAction->setIcon(QIcon(EnvUtil::resPath("zentao.ico")));
    m_quitAction->setText(tlng("tray.quit"));
    connect(m_restoryAction,SIGNAL(triggered()),this,SLOT(restory()));
    connect(m_quitAction,SIGNAL(triggered()),this,SLOT(quit()));

    m_tray->setContextMenu(m_menu);
    m_menu->addAction(m_restoryAction);
    m_menu->addAction(m_quitAction);

    m_tray->show();
}

void TrayManager::updateIcon()
{
    m_tray->setIcon(QIcon(m_ctr->logoPath()));
}

void TrayManager::adjustAfterLang()
{
    m_restoryAction->setText(tlng("tray.show"));
    m_quitAction->setText(tlng("tray.quit"));
}

void TrayManager::quit()
{
    QApplication::quit();
}

void TrayManager::restory()
{
    this->m_mainWidget->showNormal();
    this->m_mainWidget->activateWindow();
}

void TrayManager::trayIconAction(QSystemTrayIcon::ActivationReason reason)
{
    if(reason == QSystemTrayIcon::Trigger)
        restory();
}
