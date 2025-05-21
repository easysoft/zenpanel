/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "modifyport.h"

#include "base/gparams.h"

#include <QPushButton>
#include <QHBoxLayout>
#include <QWidget>
#include <QIntValidator>

#include "utils/processutil.h"

ModifyPort::ModifyPort(Service *service,QWidget *parent) : ZWidget(parent)
{
    this->m_service = service;

    preCreate();
    this->setTitle(tlng("common.modifyPort") + "(" + service->title() + ")");

    QLabel *lblPort = new QLabel();
    lblPort->setText(tlng("common.setPort"));
    m_layout->addWidget(lblPort);

    m_portEdit = new QLineEdit();
    m_portEdit->setText(service->port());
    m_layout->addWidget(m_portEdit);

    QWidget *btnGroup = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,8,0,5);
    btnLayout->setAlignment(Qt::AlignRight);
    btnGroup->setLayout(btnLayout);
    m_layout->addWidget(btnGroup);

    m_lblError = new QLabel();
    m_lblError->setStyleSheet("color:#DD7920;");
    m_lblError->setVisible(false);
    btnLayout->addWidget(m_lblError);

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

    m_portEdit->setValidator(new QIntValidator(0, 65535, this));

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(close()));
    connect(btnConfirm, SIGNAL(clicked()), this, SLOT(confirm()));

    m_isConfirm = false;

    afterCreate();
}

QString ModifyPort::port()
{
    return m_portEdit->text().trimmed();
}

bool ModifyPort::isConfirm() const
{
    return m_isConfirm;
}

void ModifyPort::confirm()
{
    QString port = m_portEdit->text().trimmed();
    if(port.isEmpty())
    {
        m_lblError->setText(tlng("common.errorPortEmpty"));
        m_lblError->setVisible(true);
        return;
    }

    if(ProcessUtil::isPortIdle(port) == false)
    {
        m_lblError->setText(tlng("common.errorPortUsed"));
        m_lblError->setVisible(true);
        return;
    }

    m_lblError->setVisible(false);

    m_isConfirm = true;

    close();
}
