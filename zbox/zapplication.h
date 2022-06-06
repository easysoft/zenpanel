#ifndef ZAPPLICATION_H
#define ZAPPLICATION_H

#include <QApplication>
#include "qtsingleapplication/QtSingleApplication"

class ZApplication : public QtSingleApplication
{
    Q_OBJECT
public:
    ZApplication(QString key, int &argc, char **argv);

    bool notify(QObject *, QEvent *);
};

#endif // ZAPPLICATION_H
