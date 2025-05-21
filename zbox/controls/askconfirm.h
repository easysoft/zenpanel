/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
