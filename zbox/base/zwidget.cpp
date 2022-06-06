#include "zwidget.h"

#include <QVariant>
#include <QBitmap>
#include <QPainter>
#include <QApplication>

ZWidget::ZWidget(QWidget *parent) : QDialog(parent)
{
}

void ZWidget::preCreate()
{
    setMinimumWidth(ts(320));

    setProperty("forUse","window");

    setWindowModality(Qt::ApplicationModal);
    setWindowFlags(Qt::FramelessWindowHint);

    m_layout = new QVBoxLayout();
    this->setLayout(m_layout);

    m_layout->setContentsMargins(ts(30),ts(20),ts(30),ts(20));

    m_titleLbl = new QLabel();
    m_layout->addWidget(m_titleLbl);

    QFrame *split = new QFrame();
    split->setFrameShape(QFrame::HLine);
    m_layout->addWidget(split);

    m_layout->addSpacing(ts(10));
}

void ZWidget::afterCreate()
{
    this->adjustSize();

    QSize mSize = size();
    QRect mainBoundary = QApplication::activeWindow()->geometry();

    int x = mainBoundary.x() + (mainBoundary.height() - mSize.width())/2;
    int y = mainBoundary.y() + (mainBoundary.height() - mSize.height())/4;

    this->move(x,y);

    QBitmap bmp(this->size());
    bmp.fill();
    QPainter p(&bmp);
    p.setPen(Qt::NoPen);
    p.setBrush(Qt::black);
    p.drawRoundedRect(bmp.rect(),8,8);
    setMask(bmp);
}


void ZWidget::setTitle(QString title)
{
    m_titleLbl->setText(title);
    setWindowTitle(title);
}
