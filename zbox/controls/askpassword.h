/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef ASKPASSWORD_H
#define ASKPASSWORD_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>

#include "base/zwidget.h"

class AskPassword : public ZWidget
{
    Q_OBJECT
public:
    explicit AskPassword(QString typeKey,QString password, QWidget *parent = nullptr);

    bool changed() const;

    QString newPassword() const;

private:
    QString m_typeKey;
    QString m_oldPassword;

    QLabel *m_lblError;
    QLineEdit *m_passwordEdit;

    bool m_changed;
    QString m_newPassword;

    void showError(QString errorDes);
    void hideError();

private slots:
    void confirm();
    void textChanged(const QString &);

};

#endif // ASKPASSWORD_H
