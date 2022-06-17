#include "modifyaccount.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>


#include "base/gparams.h"
#include "utils/constutil.h"

ModifyAccount::ModifyAccount(Service *service,QWidget *parent) : ZWidget(parent)
{

    this->m_service = service;

    preCreate();
    this->setTitle(tlng("common.modifyAccount")+ "(" + service->title() + ")");

    QLabel *lblAccount = new QLabel();
    lblAccount->setText(tlng("common.account"));
    m_layout->addWidget(lblAccount);

    m_accountEdit = new QLineEdit();
    m_accountEdit->setText(m_service->account());
    m_layout->addWidget(m_accountEdit);

    if(m_service->canChangeAccount() == false)
    {
        m_accountEdit->setEnabled(false);
    }

    QLabel *lblPassword = new QLabel();
    lblPassword->setText(tlng("common.password"));
    m_layout->addWidget(lblPassword);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setText(m_service->password());
    m_layout->addWidget(m_passwordEdit);

    m_lblError = new QLabel();
    //m_lblError->setProperty("forUse","error");
    m_lblError->setWordWrap(true);
    m_lblError->setStyleSheet("color:#DD7920;font-size:14px;");
    m_layout->addWidget(m_lblError);

    m_btnGroup = new QWidget();
    m_btnLayout = new QHBoxLayout();
    m_btnLayout->setContentsMargins(ts(0),ts(15),ts(0),ts(5));
    m_btnLayout->setAlignment(Qt::AlignRight);
    m_btnGroup->setLayout(m_btnLayout);
    m_layout->addWidget(m_btnGroup);



    m_btnLayout->addStretch();

    QPushButton *btnConfirm = new QPushButton();
    btnConfirm->setProperty("forUse","yes");
    btnConfirm->setText(tlng("common.confirm"));
    m_btnLayout->addWidget(btnConfirm);

    m_btnLayout->addSpacing(ts(10));

    QPushButton *btnCancel = new QPushButton();
    btnCancel->setProperty("forUse","no");
    btnCancel->setText(tlng("common.cancel"));
    m_btnLayout->addWidget(btnCancel);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnConfirm, SIGNAL(clicked()), this, SLOT(confirm()));

    connect(m_accountEdit, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));
    connect(m_passwordEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));


    m_isConfirm = false;
    afterCreate();
}

QString ModifyAccount::account()
{
    return m_accountEdit->text().trimmed();
}

QString ModifyAccount::password()
{
    return m_passwordEdit->text().trimmed();
}

bool ModifyAccount::isConfirm() const
{
    return m_isConfirm;
}

void ModifyAccount::showError(QString errorDes)
{
    m_lblError->setText(tlng(errorDes));
    m_btnLayout->setContentsMargins(ts(0),ts(0),ts(0),ts(5));

    m_lblError->adjustSize();
}

void ModifyAccount::hideError()
{
    m_lblError->setText("");
    m_btnLayout->setContentsMargins(ts(0),ts(15),ts(0),ts(5));

    m_lblError->adjustSize();
}

void ModifyAccount::confirm()
{
    QString account = m_accountEdit->text().trimmed();
    QString password = m_passwordEdit->text().trimmed();

    if(account.isEmpty() || password.isEmpty())
    {
        showError("common.errorUseEmpty");
        return;
    }

    QRegExp accountRx("^(?=[a-z])[a-z0-9]{4,8}$");
    if(!accountRx.exactMatch(account))
    {
        showError("common.errorAccount");
        return;
    }

    //QRegExp passwordRx("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[\\s\\S]{6,12}$");
    QRegExp passwordRx(ConstUtil::RX_PASSWORD);
    if(!passwordRx.exactMatch(password))
    {
        showError("common.errorPassword");
        return;
    }

    hideError();

    //GParams::instance()->addParam(m_oper->type().toUpper() + "_ACCOUNT_NEW",account);
    //GParams::instance()->addParam(m_oper->type().toUpper() + "_PASSWORD_NEW",password);

    m_isConfirm = true;

    close();
}

void ModifyAccount::textChanged(const QString &)
{
    hideError();
}

