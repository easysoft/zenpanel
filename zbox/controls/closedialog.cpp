/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "closedialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

CloseDialog::CloseDialog(QWidget *parent):ZWidget(parent)
{
    m_hideCloseTip = false;
    m_closeCancle = false;

    preCreate();
    this->setTitle(tlng("window.closeTipTitle"));

    createUI();
    connectUI();

    afterCreate();
}

CloseDialog::~CloseDialog()
{

}

bool CloseDialog::hideCloseTip() const
{
    return m_hideCloseTip;
}

bool CloseDialog::closeCancle() const
{
    return m_closeCancle;
}

void CloseDialog::createUI()
{
    QLabel *lblDesc = new QLabel();
    lblDesc->setText(tlng("window.closeTipDesc"));
    lblDesc->setWordWrap(true);
    this->m_layout->addWidget(lblDesc);

    m_chk = new QCheckBox();
    m_chk->setText(tlng("window.closeTipCheck"));

    this->m_layout->addWidget(m_chk);

    QHBoxLayout *btnLayout = new QHBoxLayout();
    QWidget *btnWidget = new QWidget();
    btnWidget->setLayout(btnLayout);

    btnLayout->addStretch();

    m_btnYes = new QPushButton();
    m_btnYes->setText(tlng("common.confirm"));
    m_btnYes->setProperty("forUse","yes");
    btnLayout->addWidget(m_btnYes);

    btnLayout->addSpacing(ts(10));

    m_btnCancel = new QPushButton();
    m_btnCancel->setProperty("forUse","no");
    m_btnCancel->setText(tlng("common.cancel"));
    btnLayout->addWidget(m_btnCancel);

    this->m_layout->addWidget(btnWidget);
}

void CloseDialog::connectUI()
{
    connect(m_btnYes,SIGNAL(clicked()),this,SLOT(on_yesButton_clicked()));
    connect(m_btnCancel,SIGNAL(clicked()),this,SLOT(on_noButton_clicked()));
}

void CloseDialog::on_yesButton_clicked()
{
    this->m_hideCloseTip = m_chk->isChecked();
    this->hide();
}

void CloseDialog::on_noButton_clicked()
{
    this->m_closeCancle = true;
    this->hide();
}
