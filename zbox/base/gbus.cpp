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
