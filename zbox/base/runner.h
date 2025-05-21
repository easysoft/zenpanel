/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef RUNNER_H
#define RUNNER_H

#include <QThread>
#include <QVariant>

#include "base/infoconfig.h"
#include "base/servicebase.h"
#include "base/sendproxy.h"

class Runner : public QThread,public SendProxy
{
    Q_OBJECT
public:
    Runner();

    void toSend(MsgResult result);
    void toSend(QString msg, bool error=false);

    void toSendState(QString state);

    void toSendResult(bool result);

    void sleepBetween();

protected:


signals:
    void sendMsg(QVariant);
    void sendResult(bool);
    void stateChanged(QString);
};

#endif // RUNNER_H
