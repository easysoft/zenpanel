#ifndef GPARAMS_H
#define GPARAMS_H

#include "configs/yaml2stream.h"

#include <QMap>
#include <QRect>

class GParams
{
public:
    static GParams* instance();

    float scale() const;
    void setScale(float scale);

    Yaml2Stream *lang() const;
    void setLang(Yaml2Stream *lang);

    QMap<QString, QString> params() const;
    void addParam(QString key,QString value);
    QString getParam(QString key);
    bool existParam(QString suffix, QString value);

    QString getOsArch() const;
    void setOsArch(const QString &osArch);

    QString langName() const;
    void setLangName(const QString &langName);

private:
    GParams(){};
    static GParams* m_instance;

    float m_scale=1;
    QString m_osArch;
    Yaml2Stream *m_lang;
    QString m_langName;
    QMap<QString,QString> m_params;
    QMap<QString,QString> m_changedParams;
};

#endif // GPARAMS_H
