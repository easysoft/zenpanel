/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
