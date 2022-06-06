#ifndef CLOSEDIALOG_H
#define CLOSEDIALOG_H

#include <QObject>
#include <QDialog>
#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QCheckBox>

#include "base/zwidget.h"

class CloseDialog : public ZWidget
{
    Q_OBJECT
public:
    explicit CloseDialog(QWidget *parent = nullptr);
    ~CloseDialog();

    bool hideCloseTip() const;

private:
    bool m_hideCloseTip;
    QPushButton *m_btnYes;
    QCheckBox *m_chk;

    void createUI();
    void connectUI();

private slots:
    void on_yesButton_clicked();
};

#endif // CLOSEDIALOG_H
