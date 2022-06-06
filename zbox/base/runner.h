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
