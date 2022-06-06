#ifndef MODIFYPORT_H
#define MODIFYPORT_H

#include <QWidget>
#include <QLineEdit>
#include <QLabel>
#include <QDialog>

#include "base/glang.h"
#include "base/gscale.h"
#include "base/service.h"
#include "base/zwidget.h"

class ModifyPort : public ZWidget
{
    Q_OBJECT
public:
    explicit ModifyPort(Service *service,QWidget *parent = nullptr);

    QString port();

    bool isConfirm() const;

private:
    Service *m_service;

    QLineEdit *m_portEdit;
    QLabel *m_lblError;

    bool m_isConfirm;;

private slots:
    void confirm();
};

#endif // MODIFYPORT_H
