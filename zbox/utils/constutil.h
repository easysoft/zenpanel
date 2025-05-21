/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef CONSTUTIL_H
#define CONSTUTIL_H

#include <QString>

class ConstUtil
{
public:
    static QString TOTAL_NOTINSTALLED;
    static QString TOTAL_INSTALLING;
    static QString TOTAL_INSTALLED;

    static QString SERVICE_START_PENDING;
    static QString SERVICE_RUNNING;
    static QString SERVICE_PAUSED;
    static QString SERVICE_STOP_PENDING;
    static QString SERVICE_STOPPED;
    static QString SERVICE_UNKNOWN;

    static QString STATE_INSTALL_ING;
    static QString STATE_INSTALL_SUCCESS;
    static QString STATE_INSTALL_ERROR;

    static QString STATE_UNINSTALL_ING;
    static QString STATE_UNINSTALL_SUCCESS;
    static QString STATE_UNINSTALL_ERROR;

    static QString STATE_STOP_ING;
    static QString STATE_STOP_SUCCESS;
    static QString STATE_STOP_ERROR;

    static QString STATE_START_ING;
    static QString STATE_START_SUCCESS;
    static QString STATE_START_ERROR;

    static QString STATE_INIT_SELF;

    static QString RX_PASSWORD;

    static int GRID_GAP;

    static int itemWidth(int winHeight,int scaledGap);
    static int itemHeight(int winHeight,int scaledGap);

    static QString state2Custom(QString serviceState);
};



#endif // CONSTUTIL_H
