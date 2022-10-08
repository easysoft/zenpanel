#include "hardwareinfo.h"

#include <Windows.h>
#include <QCryptographicHash>

std::string GetHardWareInfo()
{
    char szWindowsDir[MAX_PATH] = { 0 };
    GetWindowsDirectoryA(szWindowsDir, MAX_PATH);
    char* p = strchr(szWindowsDir, '\\');
    p[1] = '\0';
    char    Volume[256];//卷标名  
    char    FileSysName[256];
    DWORD   SerialNum;//序列号  
    DWORD   FileNameLength;
    DWORD   FileSysFlag;
    ::GetVolumeInformationA(szWindowsDir,
        Volume,
        256,
        &SerialNum,
        &FileNameLength,
        &FileSysFlag,
        FileSysName,
        256);
    
    SerialNum ^= 0xd4e9fb1a;

    QByteArray sn;
    sn.setNum((qlonglong)SerialNum);
    return QCryptographicHash::hash(sn, QCryptographicHash::Md5).toHex().toStdString();
}
