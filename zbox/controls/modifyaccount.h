/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
