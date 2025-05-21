/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
