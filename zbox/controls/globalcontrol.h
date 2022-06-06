#ifndef GLOBALCONTROL_H
#define GLOBALCONTROL_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QPushButton>

#include "controller.h"
#include "base/gscale.h"
#include "base/glang.h"

class GlobalControl : public QWidget,GScale,GLang
{
    Q_OBJECT
public:
    explicit GlobalControl(Controller *ctr, QWidget *parent = nullptr);

    void stateChanged();
    void adjustAfterChanged();

private:
    Controller *m_ctr;

    QHBoxLayout *m_mainLayout;

    QPushButton *m_btnNotInstall;
    QPushButton *m_btnIngInstall;
    QPushButton *m_btnUninstall;
    QPushButton *m_btnVisit;

    void preCreateUI();

    void createNotInstallUI();
    void createInstallingUI();
    void createInstalledUI();

private slots:
    void sendOneSetup();
    void sendOneStop();

    void sendVisit();

signals:
    void oneClickSetup();
    void oneClickStop();

    void clickVisit();
};

#endif // GLOBALCONTROL_H
