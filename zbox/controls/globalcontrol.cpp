#include "globalcontrol.h"
#include "utils/constutil.h"

GlobalControl::GlobalControl(Controller *ctr,QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);

    m_mainLayout = new QHBoxLayout();
    m_mainLayout->setSpacing(ts(ConstUtil::GRID_GAP));
    this->setLayout(m_mainLayout);

    this->m_ctr = ctr;

    preCreateUI();
    stateChanged();
}

void GlobalControl::stateChanged()
{
    QString appRunState = m_ctr->applicationState();

    if(appRunState == ConstUtil::TOTAL_NOTINSTALLED)
    {

        m_btnNotInstall->show();
        m_btnIngInstall->hide();
        m_btnVisit->hide();
        m_btnUninstall->hide();
    }
    else if(appRunState == ConstUtil::TOTAL_INSTALLING)
    {
        m_btnNotInstall->hide();
        m_btnIngInstall->show();
        m_btnVisit->hide();
        m_btnUninstall->hide();
    }
    else if(appRunState == ConstUtil::TOTAL_INSTALLED)
    {
        m_btnNotInstall->hide();
        m_btnIngInstall->hide();
        m_btnVisit->show();
        m_btnUninstall->show();
    }

    adjustAfterChanged();
}

void GlobalControl::adjustAfterChanged()
{
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
}

void GlobalControl::preCreateUI()
{
    createNotInstallUI();
    createInstallingUI();
    createInstalledUI();

    this->show();
}

void GlobalControl::createNotInstallUI()
{
    m_btnNotInstall = new QPushButton();
    m_btnNotInstall->setCursor(Qt::PointingHandCursor);
    m_btnNotInstall->setProperty("forUse","installBtn");

    m_mainLayout->addWidget(m_btnNotInstall);

    connect(m_btnNotInstall, SIGNAL(clicked()), this, SLOT(sendOneSetup()));
}

void GlobalControl::createInstallingUI()
{
    m_btnIngInstall = new QPushButton();
    m_btnIngInstall->setProperty("forUse","installBtn");

    m_mainLayout->addWidget(m_btnIngInstall);
}

void GlobalControl::createInstalledUI()
{
    m_btnUninstall = new QPushButton();
    m_btnUninstall->setProperty("forUse","uninstallBtn");
    m_mainLayout->addWidget(m_btnUninstall);

    m_btnVisit = new QPushButton();
    m_btnVisit->setProperty("forUse","uninstallBtn");
    m_mainLayout->addWidget(m_btnVisit);

    m_btnUninstall->setCursor(Qt::PointingHandCursor);
    m_btnVisit->setCursor(Qt::PointingHandCursor);

    connect(m_btnUninstall, SIGNAL(clicked()), this, SLOT(sendOneStop()));
    connect(m_btnVisit, SIGNAL(clicked()), this, SLOT(sendVisit()));
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
