/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef ENVUTIL_H
#define ENVUTIL_H

#include <QString>

class EnvUtil
{
public:
    static QString NEWLINE;

    static QString normalizePath(QString pathStr);

    static QString joinPath(QString rootPath,QString filePath);
    static QString getPath(QString filePath);

    static QString getRootPath();
    static void setRootPath(const QString &value);

    static QString replaceText(QString text);
    static void replaceConfig(QString fromTplPath,QString toSavePath);

    static QString colon();

private:
    static QString m_rootPath;
};

#endif // ENVUTIL_H
