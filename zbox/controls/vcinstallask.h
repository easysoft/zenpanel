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
