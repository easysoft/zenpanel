/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
