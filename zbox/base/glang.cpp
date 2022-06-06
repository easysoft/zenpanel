#include "glang.h"

#include "base/gparams.h"

#include <QStringList>

GLang::GLang()
{

}

QString GLang::tlng(QString key)
{
    Yaml2Stream *stream = GParams::instance()->lang();

    if(stream->existNode(key) == false)
    {
        QStringList strs = key.split(".");
        return  strs.length() > 0 ? strs[strs.length()-1] : key;
    }

    QString result = stream->getString(key);



    return result;
}

QString GLang::tlng(MsgResult msg)
{
    Yaml2Stream *stream = GParams::instance()->lang();

    QString msgStr = msg.output;
    if(msgStr.isEmpty() == false)
    {
        QString propKey = msg.output;
        if(msg.output.startsWith("message.") == true && stream->existNode(msg.type + "." + msg.output) == true)
        {
            propKey = msg.type + "." + msg.output;
        }

        msgStr = tlng(propKey);
    }

    int portStart = msgStr.indexOf("<%");
    int portEnd = msgStr.lastIndexOf("%>");

    if(portEnd > 0)
    {
        QString beforeStr = portStart ==0 ? "" :  msgStr.mid(0,portStart);
        QString midStr = msgStr.mid(portStart+2,portEnd-portStart+1-3);
        QString afterStr = portEnd + 1 == msgStr.length()-1 ? "" : msgStr.mid(portEnd+2);

        QString portStr = midStr;
        QString noPortStr = "";

        QStringList midList = midStr.split("|");
        if(midList.length() == 2)
        {
            portStr = midList[0];
            noPortStr = midList[1];
        }

        if(msg.port.isEmpty() == true)
        {
            msgStr = beforeStr + noPortStr + afterStr;
        }
        else
        {
            msgStr = beforeStr + portStr + afterStr;
        }
    }

    QString placeholder1 = msg.placeholder1;
    if(placeholder1.isEmpty() == false && placeholder1.startsWith("~"))
    {
        placeholder1 = tlng(placeholder1.mid(1));
    }

    QString placeholder2 = msg.placeholder2;
    if(placeholder2.isEmpty() == false && placeholder2.startsWith("~"))
    {
        placeholder2 = tlng(placeholder2.mid(1));
    }

    msgStr.replace("${0}",placeholder1);
    msgStr.replace("${1}",placeholder2);

    return msgStr;
}
