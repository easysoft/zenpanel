/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
