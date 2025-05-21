/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef GBUS_H
#define GBUS_H

#include <QObject>

class GBus : public QObject
{
    Q_OBJECT
public:
    static GBus* instance();

    void onMidified();
    void onPressed();
    void onCloseMenu();

private:
    GBus(){};
    static GBus* m_instance;

signals:
    void pressed();
    void midified();
    void closeMenu();
};

#endif // GBUS_H
