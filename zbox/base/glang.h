#ifndef GLANG_H
#define GLANG_H

#include <QString>
#include "base/inforesult.h"

class GLang
{
public:
    GLang();

    QString tlng(QString key);
    QString tlng(MsgResult msg);
};

#endif // GLANG_H
