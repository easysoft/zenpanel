#ifndef INFOMESSAGE_H
#define INFOMESSAGE_H

#include <QWidget>
#include <QDialog>
#include <QLabel>

#include "base/glang.h"
#include "base/gscale.h"

class InfoMessage : public QWidget,GScale
{
    Q_OBJECT
public:
    explicit InfoMessage(QString title,QString msgStr, QWidget *parent = nullptr);

private slots:
    void confirm();

};

#endif // INFOMESSAGE_H
