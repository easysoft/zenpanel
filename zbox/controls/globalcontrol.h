#ifndef GLOBALCONTROL_H
#define GLOBALCONTROL_H

#include <QWidget>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QComboBox>

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

    QVBoxLayout m_mainLayer;
    QPushButton m_btnStartQuickOn;

    QWidget m_SettingWidget;
    QVBoxLayout m_SettingLayout;
    QLabel m_SettingTitle;

    QHBoxLayout m_DomainLayer;
    QLineEdit m_Domain0;
    QLabel m_Dot;
    QComboBox m_Domain1;

    QLabel m_ValidDomain;
    QPushButton m_SettingSave;

    QLabel m_CurrentStatus;
    QPushButton m_StopQuickOn;
    QPushButton m_VisitQuickOnPage;
    QLabel m_Usr;
    QLineEdit m_UsrName;
    QLabel m_Pass;
    QLineEdit m_UsrPass;

    void preCreateUI();

    void createNotInstallUI();
    void createInstallingUI();
    void createInstalledUI();
    void createQuickOnUI();

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
