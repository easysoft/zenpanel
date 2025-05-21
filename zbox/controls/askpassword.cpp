/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "askpassword.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QApplication>

#include "utils/constutil.h"
#include "base/gparams.h"

AskPassword::AskPassword(QString typeKey,QString password, QWidget *parent) : ZWidget(parent)
{
    this->m_typeKey = typeKey;
    this->m_oldPassword = password;

    preCreate();
    this->setTitle(tlng("window.askPasswordTitle"));

    QLabel *lblDesc = new QLabel();
    lblDesc->setWordWrap(true);
    lblDesc->setText(tlng("window.askPasswordDesc"));
    m_layout->addWidget(lblDesc);

    m_passwordEdit = new QLineEdit();
    m_passwordEdit->setText(m_oldPassword);
    m_layout->addWidget(m_passwordEdit);

    m_lblError = new QLabel();
    m_lblError->setWordWrap(true);
    m_lblError->setStyleSheet("color:#DD7920;font-size:14px;");
    m_layout->addWidget(m_lblError);

    QWidget *btnGroup = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,8,0,5);
    btnLayout->setAlignment(Qt::AlignRight);
    btnGroup->setLayout(btnLayout);
    m_layout->addWidget(btnGroup);

//    m_lblError = new QLabel();
//    m_lblError->setStyleSheet("color:#DD7920;");
//    m_lblError->setVisible(false);
//    btnLayout->addWidget(m_lblError);

    btnLayout->addStretch();

    QPushButton *btnConfirm = new QPushButton();
    btnConfirm->setProperty("forUse","yes");
    btnConfirm->setText(tlng("common.confirm"));
    btnLayout->addWidget(btnConfirm);

    btnLayout->addSpacing(ts(10));

    QPushButton *btnCancel = new QPushButton();
    btnCancel->setProperty("forUse","no");
    btnCancel->setText(tlng("common.cancel"));
    btnLayout->addWidget(btnCancel);

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnConfirm, SIGNAL(clicked()), this, SLOT(confirm()));

    connect(m_passwordEdit, SIGNAL(textChanged(const QString &)), this, SLOT(textChanged(const QString &)));

    afterCreate();
}

bool AskPassword::changed() const
{
    return m_changed;
}

QString AskPassword::newPassword() const
{
    return m_newPassword;
}

void AskPassword::confirm()
{
    QString password = m_passwordEdit->text().trimmed();

    if(password.isEmpty())
    {
        showError("common.errorUseEmpty");
        return;
    }

    QRegExp passwordRx(ConstUtil::RX_PASSWORD);
    if(!passwordRx.exactMatch(password))
    {
        showError("common.errorPassword");
        return;
    }

    hideError();

    if(m_oldPassword != password)
    {
        GParams::instance()->addParam(m_typeKey,password);
        m_changed = true;
        m_newPassword = password;
    }

    close();
}

void AskPassword::showError(QString errorDes)
{
    m_lblError->setText(tlng(errorDes));
}

void AskPassword::hideError()
{
    m_lblError->setText("");
}

void AskPassword::textChanged(const QString &)
{
    hideError();
}
