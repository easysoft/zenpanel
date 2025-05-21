/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#include "zapplication.h"
#include <QMouseEvent>
#include <QDebug>

#include "base/gbus.h"

ZApplication::ZApplication(QString key, int &argc, char **argv):QtSingleApplication(key,argc,argv)
{

}

bool ZApplication::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::MouseButtonPress)
    {
        GBus::instance()->onPressed();
    }

    return QApplication::notify(obj,e);
}
