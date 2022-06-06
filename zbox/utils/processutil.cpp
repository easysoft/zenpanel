#include "processutil.h"
#include "utils/constutil.h"

#include <QDebug>

InfoResult ProcessUtil::excuteShell(QString command)
{
    return  excuteCommand("cmd.exe /c \"" + command + "\"");
}

InfoResult ProcessUtil::excuteCommand(QString command,QStringList params)
{
    bool ret;
    QProcess process;

    if(params.length() >0)
        process.start(command,params);
    else
        process.start(command);

    ret = process.waitForStarted();
    ret = process.waitForFinished(-1);

    bool isError;
    QString output = "";

    int exitCode = process.exitCode();
    if(exitCode == 0)
    {
        isError = false;
        QByteArray byteArray = process.readAllStandardOutput();
        output = QString::fromLocal8Bit(byteArray);
     }
    else
    {
        isError = true;
        QByteArray byteArray = process.readAll();
        output = QString::fromLocal8Bit(byteArray);

        if(output.isEmpty())
        {
            QByteArray errorByteArray = process.readAllStandardError();
            output = QString::fromLocal8Bit(errorByteArray);
        }
    }

    process.close();

    return InfoResult(isError,output);
}

QString ProcessUtil::serviceState(QString serviceName)
{
    InfoResult result = excuteCommand("sc query " + serviceName);

    if(result.error == true)
        return ConstUtil::SERVICE_UNKNOWN;

    QStringList msgList = result.output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    QString status;
    if(msgList.length() >= 3)
    {
        QString statusMsgStr = msgList[2];
        if(statusMsgStr.indexOf(ConstUtil::SERVICE_START_PENDING) > 0)  //启动中
            status = ConstUtil::SERVICE_START_PENDING;
        else if(statusMsgStr.indexOf(ConstUtil::SERVICE_RUNNING) > 0)   //已启动
            status = ConstUtil::SERVICE_RUNNING;
        else if(statusMsgStr.indexOf(ConstUtil::SERVICE_PAUSED) > 0)        //暂停
            status = ConstUtil::SERVICE_PAUSED;
        else if(statusMsgStr.indexOf(ConstUtil::SERVICE_STOP_PENDING) > 0)  //停止中
            status = ConstUtil::SERVICE_STOP_PENDING;
        else if(statusMsgStr.indexOf(ConstUtil::SERVICE_STOPPED) > 0)   //已停止
            status = ConstUtil::SERVICE_STOPPED;
        else if(statusMsgStr.indexOf("1060") > 0)
            status = ConstUtil::SERVICE_UNKNOWN;
    }

    return status;
}

QString ProcessUtil::serviceBinaryPath(QString serviceName)
{
    InfoResult result = excuteCommand("sc qc " + serviceName);

    if(result.error == true)
        return "";

    QStringList msgList = result.output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    QString binaryPath;
    if(msgList.length() >= 6)
    {
        QString binaryPathStr = msgList[5];
        int firstColon = binaryPathStr.indexOf(":");
        QString pathStr = binaryPathStr.mid(firstColon+1).trimmed();
        binaryPath = pathStr.split(" ")[0];

        binaryPath = binaryPath.replace("\"","");
    }

    return binaryPath;
}

QString ProcessUtil::exeVersion(QString exePath)
{
    if(exePath.isEmpty() || exePath.indexOf("exe") < 0)
        return "";

    InfoResult result = excuteCommand(exePath);
    if(result.error == false)
        return result.output;

    return "";
}

QString ProcessUtil::servicePID(QString serviceName)
{
    InfoResult result = excuteCommand("sc queryex " + serviceName);

    if(result.error == true)
        return "";

    QStringList msgList = result.output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    for(int i=msgList.length()-1;i>=0;i--)
    {
        QString lineStr = msgList[i];
        if(lineStr.indexOf("PID") >= 0)
        {
            QString pid = lineStr.split(":")[1].trimmed();
            return pid;
        }
    }

    return "";
}

InfoResult ProcessUtil::killService(QString pid)
{
    return excuteShell("taskkill /PID " + pid + " /F");
}

bool ProcessUtil::isPortIdle(QString port)
{
    QString key = ":" + port;

    QString command = "netstat -aon|findstr " + key;
    InfoResult result = excuteCommand("cmd",QStringList()<<"/c"<<command);

    if(result.error == true)
        return true;

    QStringList outputList = result.output.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);
    foreach(QString line,outputList)
    {
        //line.replace(QRegExp("[\\s]+"), " ");
        line = line.simplified();

        QStringList lineSegs = line.split(" ");

        if(lineSegs[1].endsWith(key))
            return false;
    }

    return true;
}


