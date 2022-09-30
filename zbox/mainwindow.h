#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSystemTrayIcon>
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QLabel>
#include <QPushButton>
#include <QAction>
#include <QScreen>

#include "traymanager.h"
#include "base/gscale.h"
#include "base/glang.h"
#include "controls/globalcontrol.h"
#include "controls/servicecontainer.h"
#include "controls/logcontainer.h"
#include "controls/apachesetting.h"
#include "base/inforesult.h"
#include "base/zpopupbutton.h"
#include "base/zoffsetmenu.h"
#include "base/zpopupmenu.h"

#include "services/quickonservice.h"

class Controller;

class MainWindow : public QMainWindow,GScale,GLang
{
    Q_OBJECT

public:
    MainWindow(Controller *ctr, QWidget *parent = nullptr);
    ~MainWindow();

    void appendMsg(MsgResult msg);
    void onAppStateChanged();
    void onServiceStateChanged(QString typeAndState);

private:
    TrayManager *m_trayMgr;
    Controller *m_ctr;

    bool m_showLog;
    QString m_themeName;
    QString m_langName;
    QPixmap m_bgPixmap;

    QLabel *m_lblLogo;
    QLabel *m_lblFootor;
    QWidget *m_leftWidget; 
    QPushButton *m_btnSwitch;

    GlobalControl *m_globalControl;
    ServiceContainer *m_serviceContainer;
    LogContainer *m_logContainer;
    ApacheSetting *m_apacheSetting;

    QVBoxLayout *m_leftLayout;

    ZPopupMenu *m_menuWidget;
    QLabel *m_menuIcon;

    ZPopupButton *m_viewServiceBtn;
    ZPopupButton *m_sysBackupBtn;
    ZPopupButton *m_languageBtn;
    ZPopupButton *m_viewHelpBtn;
    ZPopupButton *m_checkUpdateBtn;
    ZPopupButton *m_switchThemeBtn;
    ZPopupButton *m_removeServiceBtn;

    ZPopupButton *m_langZhCnAction;
    ZPopupButton *m_langZhTwAction;
    ZPopupButton *m_langENAction;

    ZPopupButton *m_themeBlueAction;
    ZPopupButton *m_themeWhiteAction;
    ZPopupButton *m_themeBlackAction;

    int m_desireHeight = 570;
    int desireWidth(int realWidth, int realHeight);

    void hideLog();
    void showLog();

    void setLogWin();
    void setNologWin();

    void loadLang();
    void changeLang(QString langName);

    void loadTheme();
    void changeTheme(QString themeName);

    void adjustAfterLang();
    void adjustAfterLangImpl();

    void createLogo();
    void createLeftUI();
    void createSetingMenu(QPushButton *btn);
    void createMainUI();
    void createRightUI();
    void createFooter();

    void makeEnvironmentPath();
    void makeBasicsInfo();

    bool m_Drag;
    QPoint m_DragPosition;
    QPoint m_preDragPos;

    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);

    QScreen* getScreen();

protected:
    void closeEvent(QCloseEvent *event);
    void paintEvent(QPaintEvent *event);


private slots:
    void toggleLog();
    void closeApp();
    void onLangChanged();
    void onThemeChanged();

    void onClickViewService();
    void onClickUninstall();
    void onClickBackup();
    void onClickViewHelp();
    void onClickUpdateHelp();

    void OneClickSetup();
    void oneClickStop();


    void clickVisit();

    void showMenu();

    void resized();

};
#endif // MAINWINDOW_H
