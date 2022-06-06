#ifndef MODIFYACCOUNT_H
#define MODIFYACCOUNT_H

#include <QWidget>
#include <QDialog>
#include <QLineEdit>
#include <QLabel>

#include "base/service.h"
#include "base/zwidget.h"

class ModifyAccount : public ZWidget
{
    Q_OBJECT
public:
    explicit ModifyAccount(Service *service,QWidget *parent = nullptr);

    QString account();
    QString password();

    bool isConfirm() const;

private:
    Service *m_service;

    QLineEdit *m_accountEdit;
    QLineEdit *m_passwordEdit;

    QLabel *m_lblError;

    bool m_isConfirm;

    QWidget *m_btnGroup;
    QHBoxLayout *m_btnLayout;

    void showError(QString errorDes);
    void hideError();

private slots:
    void confirm();
    void textChanged(const QString &);

};

#endif // MODIFYACCOUNT_H
