#ifndef ZWIDGET_H
#define ZWIDGET_H

#include <QDialog>
#include <QVBoxLayout>
#include <QLabel>
#include <QFrame>

#include "base/glang.h"
#include "base/gscale.h"

class ZWidget : public QDialog, public GScale, public GLang
{
    Q_OBJECT
public:
    explicit ZWidget(QWidget *parent = nullptr);

protected:
    QLabel *m_titleLbl;
    QVBoxLayout *m_layout;

    void preCreate();
    void afterCreate();
    void setTitle(QString title);
};

#endif // ZWIDGET_H
