#include "constutil.h"

QString ConstUtil::TOTAL_NOTINSTALLED = "notInstalled";
QString ConstUtil::TOTAL_INSTALLING = "installing";
QString ConstUtil::TOTAL_INSTALLED = "installed";

QString ConstUtil::U_SERVICE_START_PENDING = "START_PENDING";
QString ConstUtil::U_SERVICE_RUNNING = "RUNNING";
QString ConstUtil::U_SERVICE_PAUSED = "PAUSED";
QString ConstUtil::U_SERVICE_STOP_PENDING = "STOP_PENDING";
QString ConstUtil::U_SERVICE_STOPPED = "STOPPED";
QString ConstUtil::U_SERVICE_UNKNOWN = "UNKNOWN";

QString ConstUtil::STATE_INSTALL_ING = "install_ing";
QString ConstUtil::STATE_INSTALL_SUCCESS = "install_success";
QString ConstUtil::STATE_INSTALL_ERROR = "install_error";

QString ConstUtil::STATE_UNINSTALL_ING = "uninstall_ing";
QString ConstUtil::STATE_UNINSTALL_SUCCESS = "uninstall_success";
QString ConstUtil::STATE_UNINSTALL_ERROR = "uninstall_error";

QString ConstUtil::STATE_STOP_ING = "stop_ing";
QString ConstUtil::STATE_STOP_SUCCESS = "stop_success";
QString ConstUtil::STATE_STOP_ERROR = "stop_error";

QString ConstUtil::STATE_START_ING = "start_ing";
QString ConstUtil::STATE_START_SUCCESS = "start_success";
QString ConstUtil::STATE_START_ERROR = "start_error";

QString ConstUtil::STATE_INIT_SELF = "init_self";

QString ConstUtil::RX_PASSWORD = "^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)[\\s\\S]{6,12}$";

int ConstUtil::GRID_GAP = 16;


int ConstUtil::itemWidth(int winHeight,int scaledGap)
{
    int totalWidth = winHeight * 0.65;  //0.69

    return (totalWidth - scaledGap * 2)/3;;
}

int ConstUtil::itemHeight(int winHeight, int scaledGap)
{
    int totalHeight = winHeight * 0.5 * 0.42; //0.58

    return  (totalHeight - scaledGap) /2;
}

