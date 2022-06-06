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
