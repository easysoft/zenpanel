#include "mainwindow.h"

#include <QCloseEvent>
#include <QApplication>
#include <QBitmap>
#include <QBrush>
#include <QWidget>
#include <QVBoxLayout>
#include <QtMath>
#include <QMenu>
#include <QAction>
#include <QString>
#include <QMessageBox>
#include <QDesktopServices>

#include <QPainter>
#include <QPalette>
#include <QDesktopWidget>
#include <QApplication>
#include <QStyle>
#include <QStringListModel>


#include "utils/envutil.h"
#include "controller.h"
#include "base/gparams.h"
#include "base/gscale.h"
#include "utils/processutil.h"
#include "utils/constutil.h"
#include "controls/askpassword.h"
#include "servicehelper.h"
#include "base/gbus.h"
#include "runners/envrunner.h"
#include "controls/closedialog.h"
#include "spdlogwrapper.hpp"

MainWindow::MainWindow(Controller *ctr,QWidget *parent)
    : QMainWindow(parent)
    , m_QuickOnLayer()
    , m_QuickOnWidget(this)
    , m_btnStartQuickOn()
    , m_SettingWidget()
    , m_SettingLayout()
    , m_SettingTitle()
    , m_DomainLayer()
    , m_Domain0()
    , m_Dot()
    , m_Domain1()
    , m_ValidDomain()
    , m_SettingSave()
    , m_CurrentStatus()
    , m_StopQuickOn()
    , m_VisitQuickOnPage()
    , m_Usr()
    , m_UsrName()
    , m_Pass()
    , m_UsrPass()
    , m_InitMoveTimer(this)
    , m_HttpReq(this)
{
    m_showLog = true;

    m_logContainer = nullptr;

    //setWindowFlags(Qt::FramelessWindowHint);
    //setAutoFillBackground(true);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    setWindowIcon(QIcon(ctr->logoPath()));

    m_trayMgr = new TrayManager(this,ctr);
    m_trayMgr->start();

    m_ctr = ctr;

    //m_ctr->computeScale();

    m_themeName = m_ctr->themeName();
    m_langName = m_ctr->langName();

    m_globalControl = nullptr;

    m_InitMoveTimer.setInterval(3);
	m_InitMoveTimer.setSingleShot(true);
    connect(&m_InitMoveTimer, SIGNAL(timeout()), this, SLOT(OnInitMoveWindow()));

    loadLang();
    loadTheme(); 

    createLogo();
    createFooter();
    createMainUI();
    createLeftUI();
//    createRightUI();

    connect(this, SIGNAL(togglelog()), this, SLOT(toggleLog()));

    adjustAfterLang();

//    makeEnvironmentPath();

    makeBasicsInfo();

    QDesktopWidget* desktopWidget = QApplication::desktop();
    connect(desktopWidget, &QDesktopWidget::resized, this, &MainWindow::resized);

    QScreen *screen = this->getScreen();
    connect(screen, &QScreen::logicalDotsPerInchChanged, this, &MainWindow::resized);

    emit SetupQuickOnInitStatus();
    emit togglelog();

    m_InitMoveTimer.start();
}

MainWindow::~MainWindow()
{
}

QScreen* MainWindow::getScreen()
{
    int number = QApplication::desktop()->screenNumber(this);

    if(number<0)
    {
        number=0;
    }

    return QGuiApplication::screens().at(number);
}

void MainWindow::resized()
{
    m_ctr->computeScale();

    //style()->unpolish(qApp);
    //loadTheme();
    //style()->polish(qApp);

    if(m_showLog == true)
        showLog();
    else
        hideLog();

    adjustAfterLang();

    m_QuickOnWidget.move((width() - m_QuickOnWidget.width()) / 2, (height() - m_QuickOnWidget.height()) / 2);

    L_DEBUG("===================================== {0}, {1}", width(), height());
}

void MainWindow::OnHttpPostData(std::shared_ptr<std::string> url, std::shared_ptr<std::string> data, std::shared_ptr<std::string> reply)
{
    L_TRACE("############### url = {0}, data =[[[{1}]]]", url->c_str(), data->c_str());
    m_HttpReq.SetUrl(url->c_str());
    m_HttpReq.SetPost(data->c_str());

    if (!m_HttpReq.Exec(*reply))
    {
        L_ERROR("!!!!!!!!!! HTTP Exec FAILED");
        return;
    }

    L_TRACE("@@@@@@@@@@@ HTTP Exec SUCCESS");
}

void MainWindow::OnNotifyQuickOnInfo(const std::shared_ptr<std::string> domain, int http_port, int https_port)
{
    m_Domain = *domain;
    m_HttpPort = http_port;
    m_HttpsPort = https_port;

    L_TRACE("======> HttpPort: {0} -- HttpsPort: {1}", m_HttpPort, m_HttpsPort);
}

// Setup UI status
void MainWindow::OnSetupQuickOnInitStatus()
{
    m_btnStartQuickOn.setVisible(true);
    m_SettingWidget.setVisible(false);
    m_CurrentStatus.setVisible(false);
    m_StartWidget.setVisible(false);
}

void MainWindow::OnSetupQuickOnSettingStatus()
{
    m_btnStartQuickOn.setVisible(false);
    m_SettingWidget.setVisible(true);
    m_CurrentStatus.setVisible(false);
    m_StartWidget.setVisible(false);
}

void MainWindow::OnSetupQuickOnCurrentStatus(int stat)
{
    m_btnStartQuickOn.setVisible(false);
    m_SettingWidget.setVisible(false);
    m_CurrentStatus.setVisible(true);
    m_StartWidget.setVisible(false);

    switch (stat)
    {
    case ST_REQDOMAIN:
    {
        m_CurrentStatus.setText(u8"正在申请域名证书...");
    }
    break;
    case ST_CHECKHARDWARE:
    {
        m_CurrentStatus.setText(u8"正在检查硬件配置...");
    }
    break;
    case ST_CHECKSERVICE:
    {
        m_CurrentStatus.setText(u8"正在检查系统服务...");
    }
    break;
    case ST_STARTSERVICE:
    {
        m_CurrentStatus.setText(u8"正在启动渠成服务...");
    }
    break;
    default:
        break;
    }
}

void MainWindow::OnSetupQuickOnStartStatus()
{
    m_btnStartQuickOn.setVisible(false);
    m_SettingWidget.setVisible(false);
    m_CurrentStatus.setVisible(false);
    m_StartWidget.setVisible(true);
}

void MainWindow::OnCustomizeDomain(int state)
{
    L_INFO("{0} @ {1}: {2}", __FUNCTION__, __LINE__, state);
}

void MainWindow::OnInitMoveWindow()
{
    resized();
    L_DEBUG("=========>>>>>>> {0}, {1}", __FUNCTION__, __LINE__);
}

void MainWindow::hideLog()
{
//    m_btnSwitch->setText(QChar(0xe646));

    m_logContainer->hide();

    setNologWin();

    this->update();
}

void MainWindow::showLog()
{
    return;

    m_btnSwitch->setText(QChar(0xe61e));

    m_logContainer->show();

    setLogWin();

    m_logContainer->adjustAfterChanged();

    this->update();
}

void MainWindow::setLogWin()
{
    QString bgPath = EnvUtil::getPath("zbox/res/theme/" + m_themeName + "/images/extend_bg.png");
    m_bgPixmap.load(bgPath);

    resize(ts(desireWidth(m_bgPixmap.width(),m_bgPixmap.height())),ts(m_desireHeight));

    L_DEBUG("{0} @ {1}", __FUNCTION__, __LINE__);
}

void MainWindow::setNologWin()
{
    QString bgPath = EnvUtil::getPath("zbox/res/theme/" + m_themeName + "/images/shrink_bg.png");
    m_bgPixmap.load(bgPath);

    resize(ts(desireWidth(m_bgPixmap.width(),m_bgPixmap.height())),ts(m_desireHeight));

    L_DEBUG("{0} @ {1}", __FUNCTION__, __LINE__);
}

void MainWindow::toggleLog()
{
    m_showLog = !m_showLog;

    if(m_showLog == true)
        showLog();
    else
        hideLog();
}

void MainWindow::loadLang()
{
    m_ctr->loadLang(m_langName);
}

void MainWindow::changeLang(QString langName)
{
    m_langName = langName;
    loadLang();
    m_ctr->setLangName(m_langName);
    adjustAfterLang();
    m_trayMgr->adjustAfterLang();
}

void MainWindow::loadTheme()
{
    QString cssPath = EnvUtil::getPath("zbox/res/theme/" + m_themeName + "/index.css");

    QFile file(cssPath);

    if(file.exists() == false)
        return;

    file.open(QFile::ReadOnly);
    QString qss = QLatin1String(file.readAll());
    file.close();

    if(GParams::instance()->scale() != 1.0)
    {
        GScale scale;

        QRegExp rx("\\d+px", Qt::CaseInsensitive);
        rx.setMinimal(true);

        int index = -1;

        while ((index = rx.indexIn(qss, index + 1)) >= 0)
        {
            int capLen = rx.cap(0).length() - 2;
            QString snum = qss.mid(index, capLen);
            snum = QString::number(scale.ts(snum.toInt()));
            qss.replace(index, capLen, snum);
            index += snum.length();
            if (index > qss.size() - 2)
            {
                break;
            }
        }
    }
    qApp->setStyleSheet(qss);
    //this->setStyleSheet(qss);

    if(m_showLog)
        setLogWin();
    else
        setNologWin();
}

void MainWindow::changeTheme(QString themeName)
{
    m_themeName = themeName;
    loadTheme();
    m_ctr->setThemeName(themeName);

    QString pixPath = m_ctr->logoPath();
    QPixmap logoPixmap;
    logoPixmap.load(pixPath);

    logoPixmap.scaled(m_lblLogo->size(), Qt::KeepAspectRatio);
    m_lblLogo->setScaledContents(true);

    m_lblLogo->setPixmap(logoPixmap);

    setWindowIcon(QIcon(m_ctr->logoPath()));

    m_trayMgr->updateIcon();
}

void MainWindow::adjustAfterLang()
{
//    m_globalControl->adjustAfterChanged();
    m_serviceContainer->adjustAfterLangChanged();
    m_logContainer->adjustAfterChanged();
//    m_apacheSetting->adjustAfterLangChanged();

    adjustAfterLangImpl();
}

void MainWindow::adjustAfterLangImpl()
{
    QSize mainSize = this->size();

    //left ui
    QSize leftSize = m_leftWidget->size();
    m_leftWidget->move(leftSize.width(),mainSize.height()/2-leftSize.height()/2);

    //logo
    QSize logSize = m_lblLogo->size();
    m_lblLogo->move(mainSize.height()/2-logSize.width()/2,mainSize.height()/3-logSize.height()-ts(12));

    //footer
    m_lblFootor->setText(tlng(m_ctr->toolTitle()) + " " + m_ctr->toolVersion());
    m_lblFootor->adjustSize();

    QSize footorSize = m_lblFootor->size();
    m_lblFootor->move(mainSize.height()/2-footorSize.width()/2,mainSize.height()-footorSize.height()*3.9);

    //window title
    this->setWindowTitle(m_lblFootor->text());

    //main menu
    m_viewServiceBtn->setText(tlng("menu.viewService"));
    m_removeServiceBtn->setText(tlng("menu.removeService"));
    m_sysBackupBtn->setText(tlng("menu.sysBackup"));
    m_languageBtn->setText(tlng("menu.Language"));
    m_viewHelpBtn->setText(tlng("menu.viewHelp"));
    m_checkUpdateBtn->setText(tlng("menu.checkUpdate"));
    m_switchThemeBtn->setText(tlng("menu.switchTheme"));

    m_langZhCnAction->setText(tlng("menu.langZhCn"));
    m_langZhTwAction->setText(tlng("menu.langZhTw"));
    m_langENAction->setText(tlng("menu.langEN"));

    m_themeBlueAction->setText(tlng("theme.blue"));
    m_themeWhiteAction->setText(tlng("theme.green"));
    m_themeBlackAction->setText(tlng("theme.pink"));

    //============
    m_viewServiceBtn->adjustSize();
    m_removeServiceBtn->adjustSize();
    m_sysBackupBtn->adjustSize();
    m_languageBtn->adjustSize();
    m_viewHelpBtn->adjustSize();
    m_checkUpdateBtn->adjustSize();
    m_switchThemeBtn->adjustSize();

    m_langZhCnAction->adjustSize();
    m_langZhTwAction->adjustSize();
    m_langENAction->adjustSize();

    m_themeBlueAction->adjustSize();
    m_themeWhiteAction->adjustSize();
    m_themeBlackAction->adjustSize();

    m_languageBtn->adjustAfterLang();
    m_switchThemeBtn->adjustAfterLang();

    m_menuWidget->adjustAfterLang();

    // QuickOn
    m_btnStartQuickOn.setText(tlng("quickon.start"));
    m_SettingTitle.setText(tlng("quickon.title"));
    m_Dot.setText(tlng("quickon.dot"));
    auto domain = tlng("quickon.domain");
    QStringListModel* domain_list = new QStringListModel;
    domain_list->setStringList(domain.split(","));
    m_Domain1.setModel(domain_list);
    m_CustomizeDomain.setText(tlng("quickon.customize"));
    m_SettingSave.setText(tlng("quickon.save"));

    m_StopQuickOn.setText(tlng("quickon.stop"));
    m_VisitQuickOnPage.setText(tlng("quickon.visit"));
    m_Usr.setText(tlng("quickon.usr") + u8":");
    m_Pass.setText(tlng("quickon.pass") + u8":");
        
    m_leftLayout->setContentsMargins(ts(4),ts(5),ts(4),ts(5));
    m_leftLayout->setSpacing(ts(1));
}

void MainWindow::createLogo()
{
    m_lblLogo = new QLabel(this);

    m_lblLogo->resize(ts(60),ts(60));

    QString pixPath = m_ctr->logoPath();
    QPixmap logoPixmap;
    logoPixmap.load(pixPath);

    logoPixmap.scaled(m_lblLogo->size(), Qt::KeepAspectRatio);
    m_lblLogo->setScaledContents(true);
    m_lblLogo->setPixmap(logoPixmap);

//    QSize mainSize = this->size();
//    QSize logSize = m_lblLogo->size();
//    m_lblLogo->move(mainSize.height()/2-logSize.width()/2,mainSize.height()/3-logSize.height()-ts(12));
}

void MainWindow::createLeftUI()
{
    m_leftWidget = new QWidget(this);
    m_leftWidget->setProperty("forUse","wrapper");
    m_leftLayout = new QVBoxLayout(m_leftWidget);
    m_leftWidget->setLayout(m_leftLayout);

    m_leftLayout->setContentsMargins(ts(4),ts(5),ts(4),ts(5));
    m_leftLayout->setSpacing(ts(1));

    QPushButton *btnClose = new QPushButton();
    btnClose->setCursor(Qt::PointingHandCursor);
    btnClose->setProperty("forUse","iconBtn");
    btnClose->setText(QChar(0xeca0));
    m_leftLayout->addWidget(btnClose);

    m_leftLayout->setSpacing(ts(5));
//    QWidget *spacing1 = new QWidget();
//    spacing1->setProperty("forUse","spacing");
//    m_leftLayout->addWidget(spacing1);

    QFrame *split = new QFrame();
    split->setProperty("forUse","hSplit");
    split->setObjectName("hSplit");
    split->setFrameShape(QFrame::HLine);
    m_leftLayout->addWidget(split);

    m_leftLayout->setSpacing(ts(5));
//    QWidget *spacing2 = new QWidget();
//    spacing2->setProperty("forUse","spacing");
//    m_leftLayout->addWidget(spacing2);

    QPushButton *btnSetting = new QPushButton();
    btnSetting->setCursor(Qt::PointingHandCursor);
    btnSetting->setProperty("forUse","iconBtn");
    btnSetting->setText(QChar(0xe62b));
    m_leftLayout->addWidget(btnSetting);

    m_leftWidget->adjustSize();

//    QSize mainSize = this->size();
//    QSize leftSize = m_leftWidget->size();
//    m_leftWidget->move(leftSize.width(),mainSize.height()/2-leftSize.height()/2);

    m_leftWidget->show();

    createSetingMenu(btnSetting);

    connect(btnClose, SIGNAL(clicked()), this, SLOT(closeApp()));
}

void MainWindow::createSetingMenu(QPushButton *btn)
{
    //create main container
    m_menuWidget = new ZPopupMenu(this);
    //m_menuWidget->setProperty("forUse","customMenu");
    m_menuWidget->stackUnder(m_leftWidget);

    //create style icon
    QPixmap themePixmap;
    QString pixPath = EnvUtil::getPath("zbox/res/theme/" + m_ctr->themeName() + "/images/theme.png");
    themePixmap.load(pixPath);

    int iconH = ts(30);
    int iconW = (float)themePixmap.width()/(float)themePixmap.height() * (float)iconH;

    m_menuIcon = new QLabel(this);
    m_menuIcon->resize(iconW,iconH);

    themePixmap.scaled(m_menuIcon->size(), Qt::KeepAspectRatio);
    m_menuIcon->setScaledContents(true);
    m_menuIcon->setPixmap(themePixmap);

    //main function button
    m_removeServiceBtn = new ZPopupButton(m_menuWidget);
    m_viewServiceBtn = new ZPopupButton(m_menuWidget);
    m_sysBackupBtn = new ZPopupButton(m_menuWidget);
    m_languageBtn = new ZPopupButton(m_menuWidget);
    m_viewHelpBtn = new ZPopupButton(m_menuWidget);
    m_checkUpdateBtn = new ZPopupButton(m_menuWidget);
    m_switchThemeBtn = new ZPopupButton(m_menuWidget);

    m_menuWidget->add(m_removeServiceBtn);
    m_menuWidget->add(m_viewServiceBtn);

    if(m_ctr->enableBackup())
        m_menuWidget->add(m_sysBackupBtn);

    m_menuWidget->add(m_languageBtn);
    m_menuWidget->add(m_viewHelpBtn);
    m_menuWidget->add(m_checkUpdateBtn);
    m_menuWidget->add(m_switchThemeBtn);

    //language function button
    ZPopupMenu *langMenu = new ZPopupMenu(this);
    //langMenu->setProperty("forUse","customMenu");

    m_langZhCnAction = new ZPopupButton(langMenu);
    m_langZhTwAction = new ZPopupButton(langMenu);
    m_langENAction = new ZPopupButton(langMenu);

    m_langZhCnAction->setObjectName("zh-cn");
    m_langZhTwAction->setObjectName("zh-tw");
    m_langENAction->setObjectName("en");


    langMenu->add(m_langZhCnAction);
    langMenu->add(m_langZhTwAction);
    langMenu->add(m_langENAction);

    langMenu->adjustSize();

    //theme function button
    ZPopupMenu *themeMenu = new ZPopupMenu(this);
    //themeMenu->setProperty("forUse","customMenu");

    m_themeBlueAction = new ZPopupButton(themeMenu);
    m_themeWhiteAction = new ZPopupButton(themeMenu);
    m_themeBlackAction = new ZPopupButton(themeMenu);

    m_themeBlueAction->setObjectName("blue");
    m_themeWhiteAction->setObjectName("green");
    m_themeBlackAction->setObjectName("pink");

    themeMenu->add(m_themeBlueAction);
    themeMenu->add(m_themeWhiteAction);
    themeMenu->add(m_themeBlackAction);

    themeMenu->adjustSize();

    m_languageBtn->setPopup(langMenu);
    m_switchThemeBtn->setPopup(themeMenu);

    connect(m_viewServiceBtn,SIGNAL(clicked()),this,SLOT(onClickViewService()));
    connect(m_removeServiceBtn,SIGNAL(clicked()),this,SLOT(onClickUninstall()));
    connect(m_sysBackupBtn,SIGNAL(clicked()),this,SLOT(onClickBackup()));

    connect(m_langZhCnAction,SIGNAL(clicked()),this,SLOT(onLangChanged()));
    connect(m_langZhTwAction,SIGNAL(clicked()),this,SLOT(onLangChanged()));
    connect(m_langENAction,SIGNAL(clicked()),this,SLOT(onLangChanged()));

    connect(m_viewHelpBtn,SIGNAL(clicked()),this,SLOT(onClickViewHelp()));
    connect(m_checkUpdateBtn,SIGNAL(clicked()),this,SLOT(onClickUpdateHelp()));

    connect(m_themeBlueAction,SIGNAL(clicked()),this,SLOT(onThemeChanged()));
    connect(m_themeWhiteAction,SIGNAL(clicked()),this,SLOT(onThemeChanged()));
    connect(m_themeBlackAction,SIGNAL(clicked()),this,SLOT(onThemeChanged()));

    m_menuIcon->setVisible(false);
    m_menuWidget->setVisible(false);

    m_menuWidget->attachWidget(m_menuIcon);

    connect(btn, SIGNAL(pressed()), this, SLOT(showMenu()));
}

void MainWindow::showMenu()
{
    QRect geometry = m_leftWidget->geometry();
    QPoint pos(geometry.x() + geometry.width()/2,geometry.y());

    m_menuWidget->move(pos);

    QSize iconSize = m_menuIcon->size();
    QSize menuSize = m_menuWidget->size();
    m_menuIcon->move(pos.x() - iconSize.width()/4, pos.y()+menuSize.height() - iconSize.height()*0.9);

    m_menuIcon->setVisible(true);
    m_menuWidget->setVisible(true);

    QList<ZPopupButton*> themeList;
    themeList.append(m_themeBlueAction);
    themeList.append(m_themeBlackAction);
    themeList.append(m_themeWhiteAction);

    foreach(ZPopupButton* btn,themeList)
    {
        btn->setCheck(false);
        if(btn->objectName() == m_ctr->themeName())
        {
            btn->setCheck(true);
        }
    }

    QList<ZPopupButton*> lngList;
    lngList.append(m_langENAction);
    lngList.append(m_langZhCnAction);
    lngList.append(m_langZhTwAction);

    foreach(ZPopupButton* btn,lngList)
    {
        btn->setCheck(false);
        if(btn->objectName() == m_ctr->langName())
        {
            btn->setCheck(true);
        }
    }
}

void MainWindow::createMainUI()
{
//    m_globalControl = new GlobalControl(m_ctr,this);
    m_serviceContainer = new ServiceContainer(m_ctr,this);
    m_logContainer = new LogContainer(m_ctr,this);
//    m_apacheSetting = new ApacheSetting(m_ctr,this);

    m_serviceContainer->setObjectName("serviceContainer");

//    m_globalControl->show();
    m_serviceContainer->show();
//    m_apacheSetting->show();

//    connect(m_globalControl, SIGNAL(oneClickSetup()), this, SLOT(OneClickSetup()));
//    connect(m_globalControl, SIGNAL(oneClickStop()), this, SLOT(oneClickStop()));
//    connect(m_globalControl, SIGNAL(clickVisit()), this, SLOT(clickVisit()));

    connect(this, SIGNAL(SetupQuickOnInitStatus()), this, SLOT(OnSetupQuickOnInitStatus()));
    connect(&m_btnStartQuickOn, SIGNAL(clicked()), this, SIGNAL(SetupQuickOnSettingStatus()));
    connect(this, SIGNAL(SetupQuickOnSettingStatus()), this, SLOT(OnSetupQuickOnSettingStatus()));
    connect(&m_SettingSave, SIGNAL(clicked()), this, SIGNAL(SetupQuickOnStartStatus()));
    connect(this, SIGNAL(SetupQuickOnCurrentStatus(int)), this, SLOT(OnSetupQuickOnCurrentStatus(int)));
    connect(this, SIGNAL(SetupQuickOnStartStatus()), this, SLOT(OnSetupQuickOnStartStatus()));
    connect(&m_CustomizeDomain, SIGNAL(stateChanged(int)), this, SLOT(OnCustomizeDomain(int)));

    m_QuickOnWidget.setProperty("forUse","QuickOn");
    m_QuickOnWidget.setLayout(&m_QuickOnLayer);

    m_SettingWidget.setProperty("forUse","QuickOn");
    m_SettingSave.setProperty("forUse","installBtn");
    m_btnStartQuickOn.setProperty("forUse","installBtn");
    m_StopQuickOn.setProperty("forUse","installBtn");
    m_VisitQuickOnPage.setProperty("forUse","installBtn");

    m_QuickOnLayer.addWidget(&m_btnStartQuickOn, 0, Qt::AlignHCenter);
    m_QuickOnLayer.addWidget(&m_SettingWidget);
    m_SettingWidget.setLayout(&m_SettingLayout);
    m_SettingLayout.addWidget(&m_SettingTitle, 0, Qt::AlignHCenter);
    m_SettingLayout.addLayout(&m_DomainLayer);
    m_DomainLayer.addWidget(&m_Domain0);
    m_DomainLayer.addWidget(&m_Dot);
    m_DomainLayer.addWidget(&m_Domain1);
    m_SettingLayout.addWidget(&m_CustomizeDomain, 0, Qt::AlignHCenter);
    m_SettingLayout.addWidget(&m_ValidDomain, 0, Qt::AlignHCenter);
    m_SettingLayout.addWidget(&m_SettingSave);
    m_QuickOnLayer.addWidget(&m_CurrentStatus);
    m_QuickOnLayer.addWidget(&m_StartWidget);
    m_StartWidget.setLayout(&m_StartLayer);
    m_StartLayer.addLayout(&m_StartButtonsLayer);
    m_StartButtonsLayer.addWidget(&m_StopQuickOn);
    m_StartButtonsLayer.addWidget(&m_VisitQuickOnPage);
    m_StartLayer.addLayout(&m_UserLayer);
    m_UserLayer.addWidget(&m_Usr);
    m_UserLayer.addWidget(&m_UsrName);
    m_UserLayer.addWidget(&m_Pass);
    m_UserLayer.addWidget(&m_UsrPass);
}

void MainWindow::createRightUI()
{
    m_btnSwitch = new QPushButton(this);
    m_btnSwitch->setCursor(Qt::PointingHandCursor);
    m_btnSwitch->setProperty("forUse","iconBtn");
    m_btnSwitch->setProperty("forSize","big");

    connect(m_btnSwitch, SIGNAL(clicked()), this, SLOT(toggleLog()));
}

void MainWindow::createFooter()
{
    m_lblFootor = new QLabel(this);
    m_lblFootor->setObjectName("footor");
}

void MainWindow::makeEnvironmentPath()
{
    EnvRunner *runner = new EnvRunner(m_ctr->iniConfig());
    runner->start();
}

void MainWindow::makeBasicsInfo()
{
    m_ctr->applyInitConfig();

    QList<MsgResult> msgList = m_ctr->basicsMsgList();
    foreach(MsgResult msg,msgList)
    {
        m_logContainer->appendMsg(msg);
    }
}

void MainWindow::closeApp()
{
    bool hideCloseTip = m_ctr->customBoolParam("hideCloseTip",false);

    if(hideCloseTip == false)
    {
        CloseDialog *dialog = new CloseDialog();
        dialog->show();
        dialog->exec();

        if(dialog->closeCancle() == true)
        {
            delete dialog;
            return;
        }

        if(dialog->hideCloseTip() == true)
        {
            m_ctr->userConfig()->setBoolean("hideCloseTip",true);
            m_ctr->userConfig()->save();
        }

        delete dialog;
    }

    hide();
}

void MainWindow::onLangChanged()
{
    ZPopupButton *zBtn=qobject_cast<ZPopupButton*>(sender());
    QString langName = zBtn->objectName();

     changeLang(langName);
}

void MainWindow::onThemeChanged()
{
    ZPopupButton *zBtn=qobject_cast<ZPopupButton*>(sender());
    QString themeName = zBtn->objectName();

    changeTheme(themeName);
}

void MainWindow::onClickViewService()
{
//    m_menuIcon->setVisible(false);
//    m_menuWidget->setVisible(false);

    ProcessUtil::excuteShell("start services.msc");
}

void MainWindow::OneClickSetup()
{
    ServiceHelper::autoSelectPort(m_ctr->serviceList(),m_ctr->userConfig());
    ServiceHelper::checkMysqlPassword(m_ctr->queryServiceList(), m_ctr->userConfig(),this);

    m_ctr->oneSetup();
}

void MainWindow::oneClickStop()
{
    m_ctr->oneStop();
}

void MainWindow::onClickUninstall()
{
//    m_menuIcon->setVisible(false);
//    m_menuWidget->setVisible(false);

    m_ctr->oneUninstall();
}

void MainWindow::onClickBackup()
{
//    m_menuIcon->setVisible(false);
//    m_menuWidget->setVisible(false);

    m_ctr->executeBackup();
}

void MainWindow::onClickViewHelp()
{
//    m_menuIcon->setVisible(false);
//    m_menuWidget->setVisible(false);
    Yaml2Stream* ini_config = m_ctr->iniConfig();
    QDesktopServices::openUrl(QUrl(ini_config->getString("app.help.url", "http://www.zenpanel.cc/")));
}

void MainWindow::onClickUpdateHelp()
{
//    m_menuIcon->setVisible(false);
//    m_menuWidget->setVisible(false);

    Yaml2Stream* ini_config = m_ctr->iniConfig();
    QDesktopServices::openUrl(QUrl(ini_config->getString("app.update.url", "http://www.zenpanel.cc/")));
}

void MainWindow::clickVisit()
{
    if (m_Domain.empty() || !m_HttpsPort)
        return;

    QString url;
    url.append("https://").append(m_Domain.c_str()).append(":").append(std::to_string(m_HttpsPort).c_str());
    QDesktopServices::openUrl(QUrl(url));
}

void MainWindow::appendMsg(MsgResult msg)
{
    m_logContainer->appendMsg(msg);
}

void MainWindow::onAppStateChanged()
{
    if(m_globalControl != nullptr)
        m_globalControl->stateChanged();
}

void MainWindow::onServiceStateChanged(QString typeAndState)
{
    m_serviceContainer->stateChanged(typeAndState);
}

int MainWindow::desireWidth(int realWidth, int realHeight)
{
    if(realHeight == 0)
        return realWidth;

    return (realWidth / realHeight) * m_desireHeight;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    this->hide();
    event->ignore();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::SmoothPixmapTransform, true);
    //painter.setOpacity(0.5);
    painter.drawPixmap(0,0,m_bgPixmap.scaled(this->size(),Qt::KeepAspectRatioByExpanding,Qt::SmoothTransformation));
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_CustomizeDomain.isVisible())
        {
            QRect geometry = m_CustomizeDomain.geometry();
            QPoint pt = m_QuickOnWidget.mapFromGlobal(event->globalPos());
            if (geometry.contains(pt))
            {
                m_CustomizeDomain.setChecked(!m_CustomizeDomain.isChecked());
                event->accept();
                return;
            }
        }
        else if (m_btnStartQuickOn.isVisible())
        {
            QRect geometry = m_btnStartQuickOn.geometry();
            QPoint pt = m_QuickOnWidget.mapFromGlobal(event->globalPos());
            if (geometry.contains(pt))
            {
                emit SetupQuickOnSettingStatus();
                event->accept();
                return;
            }
        }
        m_Drag = true;
        m_DragPosition = event->globalPos() - this->pos();
        event->accept();
    }

    m_preDragPos = this->pos();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_Drag && (event->buttons() && Qt::LeftButton))
    {
        move(event->globalPos() - m_DragPosition);
        event->accept();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
     QPoint nowPos = this->pos();

     if(qAbs(m_preDragPos.x() - nowPos.x()) >=4 || qAbs(m_preDragPos.y() - nowPos.y()) >=4)
     {
         resized();
     }

      m_Drag=false;
}
