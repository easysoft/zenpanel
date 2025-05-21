/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "envutil.h"

#include <QDir>
#include <QTextStream>

#include "base/gparams.h"

QString EnvUtil::NEWLINE = "\r\n";
QString EnvUtil::m_rootPath = "";

QString EnvUtil::getRootPath()
{
    return EnvUtil::m_rootPath;
}

void EnvUtil::setRootPath(const QString &value)
{
    EnvUtil::m_rootPath = value;
}

QString EnvUtil::normalizePath(QString pathStr)
{
    return QDir::fromNativeSeparators(pathStr);
}

QString EnvUtil::joinPath(QString rootPath, QString filePath)
{
    QString pathStr = rootPath + filePath;
    return  normalizePath(pathStr);
}

QString EnvUtil::getPath(QString filePath)
{
    QString root = getRootPath();

    return  joinPath(root,filePath);
}


QString EnvUtil::replaceText(QString text)
{
    QMap<QString,QString> values = GParams::instance()->params();
    QList<QString> keys = values.keys();
    foreach(QString key,keys)
    {
        text = text.replace("%" + key +"%",values[key]);
    }

    return text;
}

void EnvUtil::replaceConfig(QString fromTplPath, QString toSavePath)
{
    QStringList lineStrs;

    QFile file(fromTplPath);
    file.open(QIODevice::ReadOnly | QIODevice::Text);

    while(!file.atEnd())
    {

        QByteArray lineData = file.readLine();
        QString lineStr(lineData);

        lineStr = replaceText(lineStr);
        lineStrs.append(lineStr);
    }

    QFile oldConfig(toSavePath);
    if(oldConfig.exists())
    {
        oldConfig.remove();
    }

    QFile newConfig(toSavePath);
    newConfig.open(QIODevice::WriteOnly | QIODevice::Text);

    QTextStream textStream(&newConfig);
    foreach(QString line,lineStrs)
    {
        textStream<<line;
    }

    newConfig.close();
}

QString EnvUtil::colon()
{
    QString langName = GParams::instance()->langName();
    if(langName == "en")
        return ": ";

    return "：";
}
