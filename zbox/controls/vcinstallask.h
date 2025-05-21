/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef VCINSTALLASK_H
#define VCINSTALLASK_H

#include <QDialog>

#include "base/glang.h"
#include "base/gparams.h"
#include "base/gscale.h"

class VCInstallAsk : public QDialog,GLang,GScale
{
    Q_OBJECT
public:
    VCInstallAsk(QString vcRumtime,QString title,QString msgStr);

    QString result() const;

private:
    QString m_vcRumtime;
    QString m_result;

private slots:
    void confirm();
    void cancel();
};

#endif // VCINSTALLASK_H
