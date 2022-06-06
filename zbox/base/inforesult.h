#ifndef INFORESULT_H
#define INFORESULT_H

#include <QString>
#include <QVariant>

struct InfoResult
{
    InfoResult()
    {
        this->error = false;
    }

    InfoResult(QString outputStr)
    {
        this->error = false;
        this->output = outputStr;
    }

    InfoResult(bool isError,QString outputStr)
    {
        this->error = isError;
        this->output = outputStr;
    }

    bool error;
    QString output;
};

struct MsgResult
{
    bool error;
    QString output;

    QString type;
    QString port;

    QString placeholder1;
    QString placeholder2;

    QString directOutput;
};

Q_DECLARE_METATYPE(MsgResult);

#endif // INFORESULT_H
