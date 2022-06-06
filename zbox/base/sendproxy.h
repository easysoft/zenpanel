#ifndef EVENTPROXY_H
#define EVENTPROXY_H

#include "base/inforesult.h"

class SendProxy
{
public:
    SendProxy();

    virtual void toSend(MsgResult result);
    virtual void toSendState(QString state);
};

#endif // EVENTPROXY_H
