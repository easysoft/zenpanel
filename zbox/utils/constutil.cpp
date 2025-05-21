/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "constutil.h"

QString ConstUtil::TOTAL_NOTINSTALLED = "notInstalled";
QString ConstUtil::TOTAL_INSTALLING = "installing";
QString ConstUtil::TOTAL_INSTALLED = "installed";

QString ConstUtil::SERVICE_START_PENDING = "START_PENDING";
QString ConstUtil::SERVICE_RUNNING = "RUNNING";
QString ConstUtil::SERVICE_PAUSED = "PAUSED";
QString ConstUtil::SERVICE_STOP_PENDING = "STOP_PENDING";
QString ConstUtil::SERVICE_STOPPED = "STOPPED";
QString ConstUtil::SERVICE_UNKNOWN = "UNKNOWN";

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

