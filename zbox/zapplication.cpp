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
