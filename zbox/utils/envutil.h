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
