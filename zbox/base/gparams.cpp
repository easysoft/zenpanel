/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "gparams.h"

#include <QMutex>

GParams* GParams::m_instance = nullptr;

GParams* GParams::instance()
{
    if(m_instance == nullptr)
    {
        static QMutex mutex;
        QMutexLocker locker(&mutex);

        if(m_instance == nullptr)
        {
            m_instance = new GParams();
            m_instance->setScale(1.0);
        }
    }

    return m_instance;
}

float GParams::scale() const
{
    return m_scale;
}

void GParams::setScale(float scale)
{
    m_scale = scale;
}

Yaml2Stream *GParams::lang() const
{
    return m_lang;
}

void GParams::setLang(Yaml2Stream *lang)
{
    m_lang = lang;
}

QMap<QString, QString> GParams::params() const
{
    return m_params;
}

void GParams::addParam(QString key, QString value)
{
    if(m_params.contains(key))
        m_params.remove(key);

    m_params.insert(key,value);
}

QString GParams::getParam(QString key)
{
    if(m_params.contains(key))
        return m_params[key];

    return "";
}

bool GParams::existParam(QString suffix, QString value)
{
    QList<QString> keys = m_params.keys();

    foreach(QString key,keys)
    {
        if(key.endsWith("_"+suffix) && m_params[key] == value)
            return true;
    }

    return false;
}

QString GParams::getOsArch() const
{
    return m_osArch;
}

void GParams::setOsArch(const QString &osArch)
{
    m_osArch = osArch;
}

QString GParams::langName() const
{
    return m_langName;
}

void GParams::setLangName(const QString &langName)
{
    m_langName = langName;
}


