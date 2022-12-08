#include "servicecontrol.h"

#include "runners/serviceinstaller.h"
#include "base/inforesult.h"
#include "utils/constutil.h"
#include "controls/modifyaccount.h"
#include "controls/modifyport.h"
#include "runners/modifyrunner.h"
#include "utils/processutil.h"
#include "servicehelper.h"
#include "controls/askconfirm.h"

#include <QStyle>
#include <QApplication>

#include "base/gbus.h"
#include "base/gparams.h"

ServiceControl::ServiceControl(Controller *ctr, Service *service, QWidget *parent) : QWidget(parent)
{
    setAttribute(Qt::WA_StyledBackground);
    setProperty("forUse","service");

    this->m_ctr = ctr;
    this->m_service = service;

    this->m_state = ConstUtil::STATE_INIT_SELF;
}

QString ServiceControl::type()
{
    return m_service->type();
}

void ServiceControl::init()
{
    m_mainLayout = new QVBoxLayout();
    this->setLayout(m_mainLayout);

    m_mainLayout->setContentsMargins(ts(10),ts(0),ts(0),ts(0));
    m_mainLayout->setSpacing(ts(0));

    m_firstRowLayout = new QHBoxLayout();
    m_secondRowLayout = new QHBoxLayout();

    m_firstRowLayout->setContentsMargins(ts(8),ts(4),ts(6),ts(0));
    m_firstRowLayout->setAlignment(Qt::AlignLeft);
    m_firstRowLayout->setSpacing(ts(4));

    m_secondRowLayout->setContentsMargins(ts(8),ts(0),ts(6),ts(4));
    m_secondRowLayout->setSpacing(ts(2));

    m_firstRowWidget = new QWidget();
    m_firstRowWidget->setLayout(m_firstRowLayout);

    m_secondRowWidget = new QWidget();
    m_secondRowLayout->setAlignment(Qt::AlignLeft);
    m_secondRowWidget->setLayout(m_secondRowLayout);

    m_mainLayout->addWidget(m_firstRowWidget);
    m_mainLayout->addWidget(m_secondRowWidget);

    m_lblName = new QLabel();

    m_lblName->setProperty("forUse","title");
    m_lblName->setText(m_service->title());
    m_lblName->adjustSize();
    m_firstRowLayout->addWidget(m_lblName);

    m_lblState = new QLabel(m_firstRowWidget);
    //m_lblState->setProperty("forUse","iconsuccess");
    //m_lblState->setText(QChar(0xe61f));
    m_firstRowLayout->addWidget(m_lblState);


    QLabel *preLbl = new QLabel();
    preLbl->setProperty("forUse","title");
    preLbl->setText("@");
    preLbl->adjustSize();
    m_secondRowLayout->addWidget(preLbl);
    m_lblDesc = new QLabel();
    m_lblDesc->setWordWrap(true);
    //m_lblDesc = new QTextEdit();
    m_lblDesc->setAlignment(Qt::AlignVCenter);
    m_secondRowLayout->addWidget(m_lblDesc);

    m_firstRowWidget->adjustSize();
    m_secondRowWidget->adjustSize();

    m_btnMore = new QPushButton(this);
    m_btnMore->setProperty("forUse","serviceMore");
    m_btnMore->setCursor(Qt::PointingHandCursor);
    m_btnMore->setText(QChar(0xe62e));
//    m_btnMore->adjustSize();
//    m_btnMore->move(size().width() - m_btnMore->width() - ts(2), ts(3));

    initMenu();
    refreshState();
}

void ServiceControl::adjustAfterLangChanged()
{
    m_visitAction->setText(tlng("menu.visit"));
    m_viewLogAction->setText(tlng("menu.viewLog"));
    m_ManagerMySQL->setText(tlng("menu.managerMySQL"));
    m_modifyPortAction->setText(tlng("menu.modifyPort"));
    m_modifyUserAction->setText(tlng("menu.modifyUser"));
    m_installServiceAction->setText(tlng("menu.install"));
    m_stopServiceAction->setText(tlng("menu.stopService"));
    m_reinstallAction->setText(tlng("menu.reinstall"));
    m_uninstallAction->setText(tlng("menu.uninstall"));
    m_startServiceAction->setText(tlng("menu.startService"));

    style()->unpolish(m_lblDesc);
    m_lblDesc->setProperty("forLang",m_ctr->langName());
    style()->polish(m_lblDesc);

    MsgResult result;
    result.output = m_descTpl + "";
    result.port = m_service->port();
    result.placeholder1 = m_service->port();
    QString disText = tlng(result);

    //QString disText = tlng(m_descTpl);
    disText = disText.replace("${0}",m_service->port());
    m_lblDesc->setText(disText);
    //m_lblDesc->setText("<p style='line-height:1px; width:100% ; white-space: pre-wrap; '>" + disText + "</p>");
    m_lblDesc->adjustSize();

    m_btnMore->adjustSize();
    m_btnMore->move(size().width() - m_btnMore->width() - ts(2), ts(3));

    m_menu->setOffset(-ts(2),-ts(3)-m_btnMore->size().height());
}

void ServiceControl::stateChanged(QString state)
{
    m_state = state;
    refreshState();
}

void ServiceControl::HideManagerMySQL()
{
    m_ManagerMySQL->setVisible(false);
}

void ServiceControl::ShowManagerMySQL()
{
    m_ManagerMySQL->setVisible(true);
}

void ServiceControl::initMenu()
{

    m_menu = new ZOffsetMenu(m_btnMore,this);
    m_menu->setProperty("forUse","menu");
    m_menu->setProperty("forRadius","radiusMenu");
    m_menu->setAttribute(Qt::WA_TranslucentBackground);
    m_menu->setWindowFlags(Qt::Popup | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

    //main function button
    m_visitAction = new QAction(m_menu);
    m_viewLogAction = new QAction(m_menu);
    m_ManagerMySQL = new QAction(m_menu);
    m_modifyPortAction = new QAction(m_menu);
    m_modifyUserAction = new QAction(m_menu);
    m_installServiceAction = new QAction(m_menu);
    m_uninstallAction = new QAction(m_menu);
    m_stopServiceAction = new QAction(m_menu);
    m_startServiceAction = new QAction(m_menu);
    m_reinstallAction = new QAction(m_menu);

    if(m_service->launch().isEmpty() == false) m_menu->addAction(m_visitAction);
    if(m_service->log().isEmpty() == false) m_menu->addAction(m_viewLogAction);
    if(m_service->serviceName() == "mysqlzt") m_menu->addAction(m_ManagerMySQL);
    if(m_service->canChangePort() == true) m_menu->addAction(m_modifyPortAction);
    if(m_service->canChangeUser() == true) m_menu->addAction(m_modifyUserAction);

    m_menu->addAction(m_installServiceAction);
    m_menu->addAction(m_uninstallAction);
    m_menu->addAction(m_stopServiceAction);
    m_menu->addAction(m_startServiceAction);
    m_menu->addAction(m_reinstallAction);

    m_btnMore->setMenu(m_menu);


    m_menuWidget = new QPushButton(m_menu);
    m_menuWidget->setProperty("forUse","serviceMore");
    m_menuWidget->setText(QChar(0xe62e));
    m_menuWidget->adjustSize();
    m_menuWidget->move(ts(2), ts(3));

    //QPoint gPoint = m_btnMore->mapToGlobal(QPoint(0,0));
    //QPoint lPoint = QApplication::activeWindow()->mapFromGlobal(gPoint);
    //menu->setOffset(-ts(2),-ts(3)-m_btnMore->size().height());


    connect(m_visitAction,SIGNAL(triggered()),this,SLOT(onVisitClick()));
    connect(m_viewLogAction,SIGNAL(triggered()),this,SLOT(onViewLogClick()));
    connect(m_ManagerMySQL,SIGNAL(triggered()),this,SLOT(onManagerMySQL()));

    connect(m_modifyPortAction,SIGNAL(triggered()),this,SLOT(onModifyPortClick()));
    connect(m_modifyUserAction,SIGNAL(triggered()),this,SLOT(onModifyAccountClick()));

    connect(m_installServiceAction,SIGNAL(triggered()),this,SLOT(onInstallClick()));
    connect(m_uninstallAction,SIGNAL(triggered()),this,SLOT(onUninstallClick()));
    connect(m_stopServiceAction,SIGNAL(triggered()),this,SLOT(onStopServiceClick()));
    connect(m_startServiceAction,SIGNAL(triggered()),this,SLOT(onStartServiceClick()));
    connect(m_reinstallAction,SIGNAL(triggered()),this,SLOT(onReinstallClick()));  

    connect(m_menu,SIGNAL(aboutToShow()),this,SLOT(refreshMenu()));
}

void ServiceControl::refreshState()
{
    QString icon;
    QString iconCss;
    QString lblCss;
    QString descTpl;
    QString iconSize = "normal";

    if(m_state == ConstUtil::STATE_START_ING)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.startIng";
        iconCss = "success";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_START_SUCCESS)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.startSuccess";
        iconCss = "success";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_START_ERROR)
    {
        icon = QChar(0xe61a);
        iconSize = "big";
        descTpl = "serviceStatus.startError";
        iconCss = "warning";
        lblCss = "warning";
    }
    else if(m_state == ConstUtil::STATE_INSTALL_ING)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.installIng";
        iconCss = "warning";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_INSTALL_SUCCESS)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.installSuccess";
        iconCss = "success";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_INSTALL_ERROR)
    {
        icon = QChar(0xe61a);
        iconSize = "big";
        descTpl = "serviceStatus.installError";
        iconCss = "warning";
        lblCss = "warning";
    }
    else if(m_state == ConstUtil::STATE_UNINSTALL_ING)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.uninstallIng";
        iconCss = "warning";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_UNINSTALL_SUCCESS)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.uninstallSuccess";
        iconCss = "warning";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_UNINSTALL_ERROR)
    {
        icon = QChar(0xe61a);
        iconSize = "big";
        descTpl = "serviceStatus.uninstallError";
        iconCss = "warning";
        lblCss = "warning";
    }
    else if(m_state == ConstUtil::STATE_STOP_ING)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.stopIng";
        iconCss = "warning";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_STOP_SUCCESS)
    {
        icon = QChar(0xe742);
        descTpl = "serviceStatus.stopSuccess";
        iconCss = "warning";
        lblCss = "success";
    }
    else if(m_state == ConstUtil::STATE_STOP_ERROR)
    {
        icon = QChar(0xe61a);
        iconSize = "big";
        descTpl = "serviceStatus.stopError";
        iconCss = "warning";
        lblCss = "warning";
    }
    else if(m_state == ConstUtil::STATE_INIT_SELF)
    {
        QString serviceState = m_service->queryState();
        descTpl = "serviceStatus." + serviceState.toLower();
        if(serviceState == ConstUtil::SERVICE_RUNNING)
        {
            icon = QChar(0xe742);
            iconCss = "success";
            lblCss = "success";
        }
        else
        {
            icon = QChar(0xe742);
            iconCss = "warning";
            lblCss = "success";
        }
    }

    style()->unpolish(m_lblState);
    style()->unpolish(m_lblDesc);

    m_lblState->setText(icon);
    m_lblState->setProperty("forUse","icon" + iconCss);
    m_lblState->setProperty("forColor", iconCss);
    m_lblState->setProperty("iconSize",iconSize);

    m_descTpl = descTpl;

    MsgResult result;
    result.output = descTpl;
    result.port = m_service->port();
    result.placeholder1 = m_service->port();
    QString disText = tlng(result);

    m_lblDesc->setText(disText);
    //m_lblDesc->setText("<p style='line-height:1px; width:100% ; white-space: pre-wrap; '>" + disText + "</p>");
    m_lblDesc->adjustSize();

    m_lblDesc->setProperty("forUse", "lbl" + lblCss);
    m_lblDesc->setProperty("forColor", lblCss);

    m_lblDesc->setProperty("forLang",m_ctr->langName());

    style()->polish(m_lblState);
    style()->polish(m_lblDesc);
}

void ServiceControl::onVisitClick()
{
    m_service->openBrowser();
}

void ServiceControl::onManagerMySQL()
{
    m_service->openSubUrl("adminer/");
}

void ServiceControl::onViewLogClick()
{
    m_service->openLog();
}

void ServiceControl::refreshMenu()
{
    m_visitAction->setEnabled(false);
    m_viewLogAction->setEnabled(false);
    m_ManagerMySQL->setEnabled(false);
    m_modifyPortAction->setEnabled(false);
    m_modifyUserAction->setEnabled(false);
    m_installServiceAction->setEnabled(false);
    m_stopServiceAction->setEnabled(false);
    m_reinstallAction->setEnabled(false);
    m_uninstallAction->setEnabled(false);
    m_startServiceAction->setEnabled(false);

    QString serviceState = m_service->queryState();
    bool isPreInstalled = m_ctr->isPreInstalled();

    if(m_service->ignore() == true && isPreInstalled == false)
        return;

    if(serviceState == ConstUtil::SERVICE_RUNNING)
    {
        m_visitAction->setEnabled(true);
        m_viewLogAction->setEnabled(true);
        m_ManagerMySQL->setEnabled(true);
        m_modifyPortAction->setEnabled(true);
        m_modifyUserAction->setEnabled(true);
        m_installServiceAction->setEnabled(false);
        m_stopServiceAction->setEnabled(true);
        m_reinstallAction->setEnabled(true);
        m_uninstallAction->setEnabled(true);
        m_startServiceAction->setEnabled(false);
    }
    else if(serviceState == ConstUtil::SERVICE_UNKNOWN)
    {
        m_visitAction->setEnabled(false);
        m_viewLogAction->setEnabled(false);
        m_modifyPortAction->setEnabled(false);
        m_modifyUserAction->setEnabled(false);
        m_installServiceAction->setEnabled(true);
        m_stopServiceAction->setEnabled(false);
        m_reinstallAction->setEnabled(false);
        m_uninstallAction->setEnabled(false);
        m_startServiceAction->setEnabled(false);
    }
    else if(serviceState == ConstUtil::SERVICE_PAUSED)
    {
        m_visitAction->setEnabled(false);
        m_viewLogAction->setEnabled(true);
        m_modifyPortAction->setEnabled(false);
        m_modifyUserAction->setEnabled(false);
        m_installServiceAction->setEnabled(false);
        m_stopServiceAction->setEnabled(false);
        m_reinstallAction->setEnabled(true);
        m_uninstallAction->setEnabled(true);
        m_startServiceAction->setEnabled(false);
    }
    else if(serviceState == ConstUtil::SERVICE_STOPPED)
    {
        m_visitAction->setEnabled(false);
        m_viewLogAction->setEnabled(true);
        m_modifyPortAction->setEnabled(false);
        m_modifyUserAction->setEnabled(false);
        m_installServiceAction->setEnabled(false);
        m_stopServiceAction->setEnabled(false);
        m_reinstallAction->setEnabled(true);
        m_uninstallAction->setEnabled(true);
        m_startServiceAction->setEnabled(true);
    }
    else if(serviceState == ConstUtil::SERVICE_STOP_PENDING ||
            serviceState == ConstUtil::SERVICE_START_PENDING)
    {
        m_visitAction->setEnabled(false);
        m_viewLogAction->setEnabled(false);
        m_modifyPortAction->setEnabled(false);
        m_modifyUserAction->setEnabled(false);
        m_installServiceAction->setEnabled(false);
        m_stopServiceAction->setEnabled(false);
        m_reinstallAction->setEnabled(true);
        m_uninstallAction->setEnabled(true);
        m_startServiceAction->setEnabled(false);
    }
}

void ServiceControl::onModifyPortClick()
{
    ModifyPort *winPort = new ModifyPort(m_service);
    winPort->show();
    winPort->exec();

    if(winPort->isConfirm() == false)
    {
        delete winPort;
        return;
    }

    ModifyRunner *runner = new ModifyRunner(m_ctr->mainProduct(),m_ctr->subProductList(),m_ctr->serviceList(),m_service,m_ctr->userConfig());

    connect(runner, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(runner, SIGNAL(modifySccess(QString)), this, SLOT(onModifySccess(QString)));

    runner->startPort(winPort->port());

    delete  winPort;
}

void ServiceControl::onModifySccess(QString result)
{
    //GParams::instance()->addParam(m_service->type().toUpper() + "_ACCOUNT_OLD",m_service->account());
    //GParams::instance()->addParam(m_service->type().toUpper() + "_PASSWORD_OLD",m_service->password());

    refreshState();
    onReinstallClick();

    GBus::instance()->onMidified();
}

void ServiceControl::onModifyAccountClick()
{
    ModifyAccount *winAccount = new ModifyAccount(m_service);
    winAccount->show();
    winAccount->exec();

    if(winAccount->isConfirm() == false)
    {
        delete winAccount;
        return;
    }

    QString stata = m_service->queryState();
    if(stata == ConstUtil::SERVICE_RUNNING && m_service->enableAuth() == true)
    {
        AskConfirm *confirm = new AskConfirm(tlng("window.askModifyPasswordRerunTitle"),tlng("window.askModifyRerunContent"));
        confirm->show();
        confirm->exec();

        if(confirm->isConfirm() == true)
        {
            ModifyRunner *runner = new ModifyRunner(m_ctr->mainProduct(),m_ctr->subProductList(),m_ctr->serviceList(),m_service,m_ctr->userConfig());

            connect(runner, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
            connect(runner, SIGNAL(modifySccess(QString)), this, SLOT(onModifySccess(QString)));

            runner->startUser(winAccount->account(),winAccount->password());
        }

        delete confirm;
    }
    else
    {
        m_ctr->userConfig()->setString(m_service->type()+".account",winAccount->account());
        m_ctr->userConfig()->setString(m_service->type()+".password",winAccount->password());
        m_ctr->userConfig()->save();

        m_service->extractParamValues();

        GBus::instance()->onMidified();
    }


    delete  winAccount;
}

void ServiceControl::onInstallClick()
{
    ServiceHelper::autoSelectPort(m_service,m_ctr->userConfig());
    ServiceHelper::checkMysqlPassword(m_service, m_ctr->userConfig(),this->parentWidget());

    ServiceInstaller *installer = new ServiceInstaller(m_service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onSingleInstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->installSingleService();
}

void ServiceControl::onStopServiceClick()
{
    ServiceInstaller *installer = new ServiceInstaller(m_service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onSingleStopFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->stopSingleSerivce();
}

void ServiceControl::onStartServiceClick()
{
    ServiceInstaller *installer = new ServiceInstaller(m_service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onSingleStartFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->restartSingleService();
}

void ServiceControl::onReinstallClick()
{
    ServiceInstaller *installer = new ServiceInstaller(m_service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onSingleReinstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->reinstallSingleService();
}

void ServiceControl::onUninstallClick()
{
    ServiceInstaller *installer = new ServiceInstaller(m_service);

    connect(installer, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
    connect(installer, SIGNAL(sendResult(bool)), this, SLOT(onSingleUninstallFinished(bool)));
    connect(installer, SIGNAL(stateChanged(QString)), this, SLOT(onStateChanged(QString)));

    installer->uninstallSingleService();
}

void ServiceControl::onReciveMsg(QVariant msg)
{
    MsgResult result = msg.value<MsgResult>();
    m_ctr->appendMsg(result);
}

void ServiceControl::onSingleInstallFinished(bool result)
{
    m_ctr->updateAppState();
    m_ctr->onAppStateChanged();
}

void ServiceControl::onSingleStopFinished(bool result)
{
    m_ctr->updateAppState();
    m_ctr->onAppStateChanged();
}

void ServiceControl::onSingleReinstallFinished(bool result)
{
    //do after account changed
    GParams::instance()->addParam(m_service->type().toUpper() + "_ACCOUNT_OLD",m_service->account());
    GParams::instance()->addParam(m_service->type().toUpper() + "_PASSWORD_OLD",m_service->password());

    m_ctr->updateAppState();
    m_ctr->onAppStateChanged();
}

void ServiceControl::onSingleUninstallFinished(bool result)
{
    m_ctr->updateAppState();
    m_ctr->onAppStateChanged();
}

void ServiceControl::onSingleStartFinished(bool)
{
    m_ctr->updateAppState();
    m_ctr->onAppStateChanged();
}

void ServiceControl::onStateChanged(QString typeAndState)
{
    QStringList strList = typeAndState.split("|");
    QString state = strList[1];

    stateChanged(state);
}
