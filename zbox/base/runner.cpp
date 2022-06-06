#include "runner.h"

#include <QFile>
#include <QTextStream>
#include <QDir>

#include "base/gparams.h"
#include "base/inforesult.h"
#include "utils/envutil.h"
#include "utils/processutil.h"
#include "QDebug"

Runner::Runner():
    QThread(),SendProxy()
{

}

void Runner::toSend(MsgResult result)
{
    QVariant variant;
    variant.setValue(result);

    emit sendMsg(variant);
}

void Runner::toSendState(QString state)
{
    emit stateChanged(state);
}

void Runner::toSendResult(bool result)
{
    emit sendResult(result);
}

void Runner::toSend(QString msg, bool error)
{
    MsgResult result;
    result.output = msg;
    result.error = error;

    toSend(result);
}

void Runner::sleepBetween()
{
    msleep(100);
}
