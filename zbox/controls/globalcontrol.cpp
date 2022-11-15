#include "globalcontrol.h"
#include "utils/constutil.h"

#include "spdlogwrapper.hpp"

GlobalControl::GlobalControl(Controller *ctr,QWidget *parent) 
    : QWidget(parent)
    , m_mainLayer(this)
    , m_btnStartQuickOn(this)
    , m_SettingWidget(this)
    , m_SettingLayout()
    , m_SettingTitle()
    , m_DomainLayer()
    , m_Domain0()
    , m_Dot()
    , m_Domain1()
    , m_ValidDomain()
    , m_SettingSave()
    , m_CurrentStatus()
    , m_StopQuickOn()
    , m_VisitQuickOnPage()
    , m_Usr()
    , m_UsrName()
    , m_Pass()
    , m_UsrPass()
{
    setAttribute(Qt::WA_StyledBackground);

    this->setLayout(&m_mainLayer);
    this->m_ctr = ctr;

    L_TRACE("!!!!!!!! w = {0},  h = {1}", width(), height());

    m_mainLayer.setSpacing(0);
    m_mainLayer.setContentsMargins(10, 20, 0, 0);
    
    preCreateUI();
    stateChanged();
}

void GlobalControl::stateChanged()
{
    QString appRunState = m_ctr->applicationState();

    if(appRunState == ConstUtil::TOTAL_NOTINSTALLED)
    {
        /*
        m_btnNotInstall->show();
        m_btnIngInstall->hide();
        m_btnVisit->hide();
        m_btnUninstall->hide();
        */
    }
    else if(appRunState == ConstUtil::TOTAL_INSTALLING)
    {
        /*
        m_btnNotInstall->hide();
        m_btnIngInstall->show();
        m_btnVisit->hide();
        m_btnUninstall->hide();
        */
    }
    else if(appRunState == ConstUtil::TOTAL_INSTALLED)
    {
        /*
        m_btnNotInstall->hide();
        m_btnIngInstall->hide();
        m_btnVisit->show();
        m_btnUninstall->show();
        */
    }

    adjustAfterChanged();
}

void GlobalControl::adjustAfterChanged()
{
    /*
    m_btnVisit->setMinimumWidth(0);
    m_btnUninstall->setMinimumWidth(0);

    m_btnNotInstall->setMinimumWidth(0);
    m_btnIngInstall->setMinimumWidth(0);

    m_btnNotInstall->setText(tlng("window.notInstalled"));
    m_btnIngInstall->setText(tlng("window.installing"));

    m_btnVisit->setText(tlng("window.installed.visit"));
    m_btnUninstall->setText(tlng("window.installed.stop"));

    m_btnVisit->adjustSize();
    m_btnUninstall->adjustSize();

    m_btnNotInstall->adjustSize();
    m_btnIngInstall->adjustSize();

    QSize mainSize = parentWidget()->size();

    int itemWidth = ConstUtil::itemWidth(mainSize.height(),ts(ConstUtil::GRID_GAP));

    itemWidth = (itemWidth * 3 + ts(ConstUtil::GRID_GAP))/2;


    m_btnVisit->setMinimumWidth(itemWidth);
    m_btnUninstall->setMinimumWidth(itemWidth);

    m_btnNotInstall->setMinimumWidth(itemWidth);
    m_btnIngInstall->setMinimumWidth(itemWidth);

    //m_btnNotInstall->setMinimumWidth(itemWidth*2 + ts(ConstUtil::GRID_GAP));
    //m_btnIngInstall->setMinimumWidth(itemWidth*2 + ts(ConstUtil::GRID_GAP));

    this->adjustSize();

    QSize nowSize = size();
    this->move(mainSize.height()/2 - nowSize.width()/2,mainSize.height()/3);
    */
}

void GlobalControl::preCreateUI()
{
    createNotInstallUI();
    createInstallingUI();
    createInstalledUI();
    createQuickOnUI();

    this->show();
}

void GlobalControl::createNotInstallUI()
{
    m_btnStartQuickOn.setCursor(Qt::PointingHandCursor);
//    m_btnStartQuickOn.setProperty("forUse","installBtn");

    m_mainLayer.addWidget(&m_btnStartQuickOn);

    connect(&m_btnStartQuickOn, SIGNAL(clicked()), this, SLOT(sendOneSetup()));
}

void GlobalControl::createInstallingUI()
{
    /*
    m_btnIngInstall = new QPushButton();
    m_btnIngInstall->setProperty("forUse","installBtn");

    m_mainLayout->addWidget(m_btnIngInstall);
    */
}

void GlobalControl::createInstalledUI()
{
    /*
    m_btnUninstall = new QPushButton();
    m_btnUninstall->setProperty("forUse","uninstallBtn");
    m_mainLayout->addWidget(m_btnUninstall);

    m_btnVisit = new QPushButton();
    m_btnVisit->setProperty("forUse","uninstallBtn");
    m_btnVisit->setVisible(false);
    m_mainLayout->addWidget(m_btnVisit);

    m_btnUninstall->setCursor(Qt::PointingHandCursor);
    m_btnVisit->setCursor(Qt::PointingHandCursor);

    m_btnVisit->hide();

    connect(m_btnUninstall, SIGNAL(clicked()), this, SLOT(sendOneStop()));
    connect(m_btnVisit, SIGNAL(clicked()), this, SLOT(sendVisit()));
    */
}

void GlobalControl::createQuickOnUI()
{
    /*
    m_QuickOnLayout = new QVBoxLayout;
    m_QuickOnWidget = new QWidget(this);
    m_QuickOnWidget->setProperty("forUse","quickon");
    m_QuickOnTitle = new QLabel(m_QuickOnWidget);
    m_QuickOnTitle->setText("title");
    
    m_QuickOnDomainLayout = new QHBoxLayout(m_QuickOnWidget);
    m_QuickOnDomain0 = new QLineEdit(m_QuickOnWidget);
    m_QuickOnDot = new QLabel(m_QuickOnWidget);
    m_QuickOnDomain1 = new QComboBox(m_QuickOnWidget);

    m_QuickOnSave = new QPushButton(m_QuickOnWidget);
    m_QuickOnSave->setProperty("forUse","uninstallBtn");
    m_QuickOnSave->setCursor(Qt::PointingHandCursor);

    m_QuickOnLayout->addWidget(m_QuickOnTitle);
    
    m_QuickOnDomainLayout->addWidget(m_QuickOnDomain0);
    m_QuickOnDomainLayout->addWidget(m_QuickOnDot);
    m_QuickOnDomainLayout->addWidget(m_QuickOnDomain1);
    m_QuickOnLayout->addLayout(m_QuickOnDomainLayout);
    
    m_QuickOnLayout->addWidget(m_QuickOnSave);

    m_QuickOnLayout->setStretchFactor(m_QuickOnTitle, 1);
    m_QuickOnLayout->setStretchFactor(m_QuickOnDomainLayout, 3);
    m_QuickOnLayout->setStretchFactor(m_QuickOnSave, 2);

    m_QuickOnSave->setText("save");

    m_QuickOnWidget->setLayout(m_QuickOnLayout);

    m_mainLayout->addWidget(m_QuickOnWidget);
    */
}

void GlobalControl::sendOneSetup()
{
    emit oneClickSetup();
}

void GlobalControl::sendOneStop()
{
    emit oneClickStop();
}

void GlobalControl::sendVisit()
{
    emit clickVisit();
}
