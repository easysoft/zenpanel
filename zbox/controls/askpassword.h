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
