#ifndef SERVICECONTROL_H
#define SERVICECONTROL_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QMenu>
#include <QAction>
#include <QTextEdit>

#include "controller.h"
#include "base/gscale.h"
#include "base/glang.h"
#include "base/zoffsetmenu.h"

#include <QLineEdit>
#include <QComboBox>

class ServiceControl : public QWidget,GScale,GLang
{
    Q_OBJECT
public:
    explicit ServiceControl(Controller *ctr, Service *service, QWidget *parent = nullptr);

    QString type();

    void init();
    void adjustAfterLangChanged();
    void stateChanged(QString typeAndState);

private:
    QString m_state;
    QString m_descTpl;

    Controller *m_ctr;
    Service *m_service;

/*
    QVBoxLayout *m_mainLayout;
    QHBoxLayout *m_firstRowLayout;
    QHBoxLayout *m_secondRowLayout;

    QWidget *m_firstRowWidget;
    QWidget *m_secondRowWidget;

    QLabel *m_lblName;
    QLabel *m_lblState;
    QPushButton *m_btnMore;

    QLabel *m_lblDesc;
    //QTextEdit *m_lblDesc;

    QPushButton *m_menuWidget;

    QAction *m_visitAction;
    QAction *m_viewLogAction;
    QAction *m_modifyPortAction;
    QAction *m_modifyUserAction;
    QAction *m_installServiceAction;
    QAction *m_uninstallAction;
    QAction *m_stopServiceAction;
    QAction *m_startServiceAction;
    QAction *m_reinstallAction;

    ZOffsetMenu *m_menu;

    void initMenu();
*/
    void refreshState();


private slots:
    void onVisitClick();
    void onViewLogClick();

    void onModifyPortClick();
    void onModifyAccountClick();

    void refreshMenu();

    void onInstallClick();
    void onStopServiceClick();
    void onStartServiceClick();
    void onReinstallClick();
    void onUninstallClick();

    void onReciveMsg(QVariant msg);
    void onSingleInstallFinished(bool result);
    void onSingleStopFinished(bool result);
    void onSingleReinstallFinished(bool result);
    void onSingleUninstallFinished(bool result);
    void onSingleStartFinished(bool);

    void onStateChanged(QString typeAndState);
    void onModifySccess(QString);

signals:

};

#endif // SERVICECONTROL_H
