#include "closedialog.h"

#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>

CloseDialog::CloseDialog(QWidget *parent):ZWidget(parent)
{
    m_hideCloseTip = false;

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

    this->m_layout->addWidget(btnWidget);
}

void CloseDialog::connectUI()
{
    connect(m_btnYes,SIGNAL(clicked()),this,SLOT(on_yesButton_clicked()));
}

void CloseDialog::on_yesButton_clicked()
{
    this->m_hideCloseTip = m_chk->isChecked();
    this->hide();
}
