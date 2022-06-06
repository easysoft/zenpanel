#ifndef ZWIDGETBUTTON_H
#define ZWIDGETBUTTON_H

#include <QPushButton>
#include <QLabel>

#include "base/glang.h"
#include "base/gscale.h"

class ZPopupMenu;

class ZPopupButton : public QPushButton,public GScale
{
    Q_OBJECT
public:
    ZPopupButton(ZPopupMenu *parent);

    void setPopup(ZPopupMenu *widget);
    void closePopup();

    void adjustAfterLang();

    bool hasPopup();
    bool popContainCursor();

    bool check() const;
    void setCheck(bool check);

private:
    ZPopupMenu *m_menu=nullptr;
    ZPopupMenu *m_popWidget=nullptr;

    QLabel *m_checkLbl;
    bool m_check;

protected:
    void enterEvent(QEvent *event);

private slots:
    void onClicked();
};

#endif // ZWIDGETBUTTON_H
