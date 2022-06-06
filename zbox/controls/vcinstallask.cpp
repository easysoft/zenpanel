#include "vcinstallask.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

#include "base/gparams.h"

VCInstallAsk::VCInstallAsk(QString vcRumtime,QString title,QString msgStr):QDialog()
{
    setProperty("forUse","window");

    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint&~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(ts(20),ts(10),ts(20),ts(10));
    mainLayout->setSpacing(ts(10));

    setWindowTitle(tlng(title));

    QLabel *lbl = new QLabel();
    lbl->setText(tlng(msgStr));
    mainLayout->addWidget(lbl);

    QWidget *btnWidget = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnWidget->setLayout(btnLayout);
    btnLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(btnWidget);

    btnLayout->addStretch();

    QPushButton *btnOk = new QPushButton();
    btnOk->setProperty("forUse","yes");
    btnOk->setText(tlng("common.confirm"));
    btnLayout->addWidget(btnOk);

    QPushButton *btnCancel = new QPushButton();
    btnCancel->setProperty("forUse","yes");
    btnCancel->setText(tlng("common.cancel"));
    btnLayout->addWidget(btnCancel);

    this->adjustSize();

    connect(btnOk, SIGNAL(clicked()), this, SLOT(confirm()));
    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
}

QString VCInstallAsk::result() const
{
    return m_result;
}

void VCInstallAsk::confirm()
{
    m_result = "confirm";
    close();
}

void VCInstallAsk::cancel()
{
    m_result = "cancel";
    close();
}
