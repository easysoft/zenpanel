/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef APATCHEACCOUNTSETTING_H
#define APATCHEACCOUNTSETTING_H

#include <QWidget>
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QCommandLinkButton>

#include "base/gscale.h"
#include "base/glang.h"
#include "controller.h"
#include "base/servicebase.h"

class ApacheSetting : public QWidget,GLang,GScale
{
    Q_OBJECT
public:
    explicit ApacheSetting(Controller *ctr, QWidget *parent = nullptr);

    void adjustAfterLangChanged();

private:
    Controller *m_ctr;

    QCheckBox *m_chbox;
    QLabel *m_lblAccount;
    QLabel *m_lblPassword;

    QPushButton *m_btnCopyPassword;
    QPushButton *m_btnModifyPassword;

    Service *m_appacheService = nullptr;

private slots:
    void settingChanged(int state);
    void midified();
    void copyPassword();
    void modifyPassword();
    void onModifySccess(QString result);
    void onReciveMsg(QVariant msg);
};

#endif // APATCHEACCOUNTSETTING_H
