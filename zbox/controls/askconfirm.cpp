#include "askconfirm.h"

AskConfirm::AskConfirm(QString title,QString content, QWidget *parent)
    :ZWidget(parent)
{
    preCreate();
    this->setTitle(title);

    QLabel *contentLabel = new QLabel();
    contentLabel->setWordWrap(true);
    contentLabel->setText(content);
    m_layout->addWidget(contentLabel);

    QWidget *btnGroup = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnLayout->setContentsMargins(0,8,0,5);
    btnLayout->setAlignment(Qt::AlignRight);
    btnGroup->setLayout(btnLayout);
    m_layout->addWidget(btnGroup);

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

    connect(btnCancel, SIGNAL(clicked()), this, SLOT(cancel()));
    connect(btnConfirm, SIGNAL(clicked()), this, SLOT(confirm()));

    m_isConfirm = false;
    afterCreate();
}

bool AskConfirm::isConfirm() const
{
    return m_isConfirm;
}

void AskConfirm::cancel()
{
    m_isConfirm = false;

    close();
}

void AskConfirm::confirm()
{
    m_isConfirm = true;

    close();
}
