/**
 * ZenPanel - One-click configurable service control panel with integrated Windows runtime environments.
 *
 * @copyright Copyright 2009-2025 禅道软件（青岛）有限公司(ZenTao Software (Qingdao) Co., Ltd. www.chandao.com)
 * @license   LGPL v3.0 - Full terms in LICENSE file.
 * @notice    This program uses Qt which is licensed under LGPL v3.
 */
#ifndef LOGCONTAINER_H
#define LOGCONTAINER_H

#include <QWidget>
#include <QHBoxLayout>
#include <QPushButton>
#include <QButtonGroup>
#include <QFrame>
#include <QTextEdit>

#include "controller.h"
#include "base/gscale.h"
#include "base/glang.h"
#include "base/inforesult.h"

class LogContainer : public QWidget,GScale,GLang
{
    Q_OBJECT
public:
    explicit LogContainer(Controller *ctr,QWidget *parent = nullptr);

    void adjustAfterChanged();
    void appendMsg(MsgResult msg);
    void appendMsg(QString msg);
    void clearMsgs();

private:
    Controller *m_ctr;

    float m_overRatio;

    QWidget *m_tabWidget;
    QHBoxLayout *m_tabLayout;
    QPushButton *m_btnLog;
    QButtonGroup *m_btnGroup;

    QList<MsgResult> m_sysLogResults;
    QTextEdit *m_text;

    int m_tabIndex;

    void location();
    void createUI();
    void adjustLog();

    void addSeperator();

    void showSysLog();
    void ShowServiceLog(Service *service);

    QString transText(MsgResult result);

private slots:
    void serviceClicked(int index);

};

#endif // LOGCONTAINER_H
