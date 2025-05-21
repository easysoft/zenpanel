/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
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
