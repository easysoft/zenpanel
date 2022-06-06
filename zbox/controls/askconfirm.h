#ifndef ASKCONFIRM_H
#define ASKCONFIRM_H

#include "base/zwidget.h"

#include <QPushButton>
#include <QLabel>

class AskConfirm : public ZWidget
{
    Q_OBJECT
public:
    AskConfirm(QString title,QString content, QWidget *parent = nullptr);

    bool isConfirm() const;

private:
    bool m_isConfirm;

private slots:
    void cancel();
    void confirm();
};

#endif // ASKCONFIRM_H
