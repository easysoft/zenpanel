#include "infomessage.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>

InfoMessage::InfoMessage(QString title,QString msgStr, QWidget *parent) : QWidget(parent)
{
    setProperty("forUse","window");

    setWindowFlags(windowFlags()&~Qt::WindowMinMaxButtonsHint&~Qt::WindowContextHelpButtonHint);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    this->setLayout(mainLayout);

    mainLayout->setAlignment(Qt::AlignCenter);
    mainLayout->setContentsMargins(ts(20),ts(10),ts(20),ts(10));
    mainLayout->setSpacing(ts(10));

    setWindowTitle(title);

    QLabel *lbl = new QLabel();
    lbl->setText(msgStr);
    mainLayout->addWidget(lbl);

    QWidget *btnWidget = new QWidget();
    QHBoxLayout *btnLayout = new QHBoxLayout();
    btnWidget->setLayout(btnLayout);
    btnLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(btnWidget);

    btnLayout->setAlignment(Qt::AlignHCenter);

    QPushButton *btn = new QPushButton();
    btn->setProperty("forUse","yes");
    btn->setText("确定");
    btnLayout->addWidget(btn);


    this->adjustSize();

    connect(btn, SIGNAL(clicked()), this, SLOT(confirm()));
}

void InfoMessage::confirm()
{
    close();
}
