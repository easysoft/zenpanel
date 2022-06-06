#ifndef ZMENU_H
#define ZMENU_H

#include <QMenu>
#include <QPushButton>

class ZOffsetMenu : public QMenu
{
    Q_OBJECT
public:
    explicit ZOffsetMenu(QPushButton *refer = nullptr, QWidget *parent = nullptr);

    void setOffset(int x, int y);

protected:
    bool m_setOffset;
    QPoint m_offset;

    QPushButton *m_referBtn;
    void showEvent(QShowEvent *event);
};

#endif // ZMENU_H
