/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "apachesetting.h"

#include "base/gparams.h"
#include "utils/constutil.h"
#include "utils/envutil.h"
#include "base/gbus.h"
#include "controls/modifyaccount.h"
#include "runners/ModifyRunner.h"
#include "base/inforesult.h"
#include "controls/askconfirm.h"
#include "runners/serviceinstaller.h"

#include <QClipboard>
#include <QApplication>

ApacheSetting::ApacheSetting(Controller *ctr, QWidget *parent) : QWidget(parent)
{
    m_ctr = ctr;

    QHBoxLayout *selfLayout = new QHBoxLayout();
    this->setLayout(selfLayout);

    m_chbox = new QCheckBox();
    selfLayout->addWidget(m_chbox);

    m_lblAccount = new QLabel();
    m_lblAccount->setProperty("forUse","setting");
    selfLayout->addWidget(m_lblAccount);

    m_lblPassword = new QLabel();
    m_lblPassword->setProperty("forUse","setting");
    selfLayout->addWidget(m_lblPassword);

    m_btnModifyPassword = new QPushButton();
    m_btnModifyPassword->setProperty("forUse","link");
    m_btnModifyPassword->setCursor(Qt::PointingHandCursor);
    selfLayout->addWidget(m_btnModifyPassword);

    m_btnCopyPassword = new QPushButton();
    m_btnCopyPassword->setProperty("forUse","link");
    m_btnCopyPassword->setCursor(Qt::PointingHandCursor);
    selfLayout->addWidget(m_btnCopyPassword);

    m_appacheService = m_ctr->apacheService();

    m_chbox->setChecked(m_appacheService->enableAuth());

    connect(m_chbox,SIGNAL(stateChanged(int)),this,SLOT(settingChanged(int)));
    connect(GBus::instance(),SIGNAL(midified()),this,SLOT(midified()));

    connect(m_btnCopyPassword, SIGNAL(clicked()), this, SLOT(copyPassword()));
    connect(m_btnModifyPassword, SIGNAL(clicked()), this, SLOT(modifyPassword()));
}

void ApacheSetting::adjustAfterLangChanged()
{
    m_chbox->setText(tlng("window.apacheUser"));

    QString account = m_appacheService->account();
    QString password = m_appacheService->password();

    m_lblAccount->setText(tlng("common.account") + EnvUtil::colon() + account);
    m_lblPassword->setText(tlng("common.password") + EnvUtil::colon() + password);

    m_btnCopyPassword->setText(tlng("common.copyPassword"));
    m_btnModifyPassword->setText(tlng("common.modifyPassword"));

    //m_btnCopyPassword->setText("<u>" + tlng("common.copyPassword") + "</u>");
    //m_btnModifyPassword->setText("<u>" + tlng("common.modifyPassword") + "</u>");

    m_chbox->adjustSize();
    m_lblAccount->adjustSize();
    m_lblPassword->adjustSize();
    m_btnCopyPassword->adjustSize();
    m_btnModifyPassword->adjustSize();

    adjustSize();

    int gap = ts(ConstUtil::GRID_GAP);
    QSize selfSize = this->size();
    QWidget *sContainer = this->parent()->findChild<QWidget*>("serviceContainer");

    QRect sRect = sContainer->geometry();

    this->move( (sRect.x() + sRect.width()/2) - selfSize.width()/2,sRect.y() + sRect.height() + gap);
}

void ApacheSetting::settingChanged(int state)
{
    bool userEnabled = m_chbox->isChecked();
    m_ctr->userConfig()->setBoolean(m_appacheService->type().toLower() + ".enableAuth",userEnabled);
    m_ctr->userConfig()->save();

    m_appacheService->extractParamValues();

    QString stata = m_appacheService->queryState();
    if(stata == ConstUtil::SERVICE_RUNNING)
    {
        AskConfirm *confirm = new AskConfirm(tlng("window.askModifyAuthEnableTitle"),tlng("window.askModifyAuthEnableContnt"));
        confirm->show();
        confirm->exec();

        if(confirm->isConfirm() == true)
        {
            //SendProxy *sendProxy = new SendProxy();
            //m_appacheService->uninstallService(sendProxy);
            m_ctr->reStart(m_appacheService);
        }

        delete confirm;

    }
}

void ApacheSetting::midified()
{
    adjustAfterLangChanged();
}

void ApacheSetting::copyPassword()
{
    QString password = m_appacheService->password();

    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(password);
}

void ApacheSetting::modifyPassword()
{
    ModifyAccount *winAccount = new ModifyAccount(m_appacheService);
    winAccount->show();
    winAccount->exec();

    if(winAccount->isConfirm() == false)
    {
        delete winAccount;
        return;
    }

    QString stata = m_appacheService->queryState();
    if(stata == ConstUtil::SERVICE_RUNNING && m_appacheService->enableAuth() == true)
    {
        AskConfirm *confirm = new AskConfirm(tlng("window.askModifyPasswordRerunTitle"),tlng("window.askModifyRerunContent"));
        confirm->show();
        confirm->exec();

        if(confirm->isConfirm() == true)
        {
            ModifyRunner *runner = new ModifyRunner(m_ctr->mainProduct(),m_ctr->subProductList(),m_ctr->serviceList(),m_appacheService,m_ctr->userConfig());
            connect(runner, SIGNAL(sendMsg(QVariant)), this, SLOT(onReciveMsg(QVariant)));
            connect(runner, SIGNAL(modifySccess(QString)), this, SLOT(onModifySccess(QString)));
            runner->startUser(winAccount->account(),winAccount->password());
        }

        delete confirm;
    }
    else
    {
        m_ctr->userConfig()->setString(m_appacheService->type()+".account",winAccount->account());
        m_ctr->userConfig()->setString(m_appacheService->type()+".password",winAccount->password());
        m_ctr->userConfig()->save();

        m_appacheService->extractParamValues();

        GBus::instance()->onMidified();
    }

    delete  winAccount;
}

void ApacheSetting::onModifySccess(QString result)
{
    m_ctr->reStart(m_appacheService);

    GBus::instance()->onMidified();
}

void ApacheSetting::onReciveMsg(QVariant msg)
{
    MsgResult result = msg.value<MsgResult>();
    m_ctr->appendMsg(result);
}
