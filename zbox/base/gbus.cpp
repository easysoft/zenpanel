/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "gbus.h"

#include <QMutex>

GBus* GBus::m_instance = nullptr;

GBus* GBus::instance()
{
    if(m_instance == nullptr)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);

        if(m_instance == nullptr)
        {
            m_instance = new GBus();
        }
    }

    return m_instance;
}

void GBus::onMidified()
{
    emit midified();
}

void GBus::onPressed()
{
    emit pressed();
}

void GBus::onCloseMenu()
{
    emit closeMenu();
}
