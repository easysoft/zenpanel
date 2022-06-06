#include "logcontainer.h"

#include <QTextBlock>

LogContainer::LogContainer(Controller *ctr,QWidget *parent) : QWidget(parent)
{
    this->m_ctr = ctr;

    m_tabIndex = 0;
    m_overRatio = 0.1;

    //location();
    createUI();

    m_btnLog->setChecked(true);
    showSysLog();

    adjustAfterChanged();
}

void LogContainer::location()
{
    QSize mainSize = parentWidget()->size();
    int nowWidth = mainSize.width() - mainSize.height() + mainSize.height() * m_overRatio;
    int nowHeight = mainSize.height();

    int x = mainSize.width() - nowWidth;
    int y = 0;

    this->setMinimumSize(nowWidth,nowHeight);
    this->setMaximumSize(nowWidth,nowHeight);

    this->move(x,y);
}

void LogContainer::createUI()
{
   m_btnGroup = new QButtonGroup(this);

   m_tabWidget = new QWidget(this);
   m_tabWidget->setProperty("forUse","tabContainer");

   m_tabLayout = new QHBoxLayout();
   m_tabLayout->setAlignment(Qt::AlignLeft);
   m_tabLayout->setContentsMargins(0,0,0,ts(8));
   m_tabLayout->setSpacing(ts(12));

   m_tabWidget->setLayout(m_tabLayout);

   m_btnLog = new QPushButton();
   m_btnLog->setProperty("forUse","tabBtn");
   m_btnLog->setCursor(Qt::PointingHandCursor);
   m_btnLog->setCheckable(true);
   m_tabLayout->addWidget(m_btnLog);
   m_btnGroup->addButton(m_btnLog,0);

   QList<Service*> serviceList = m_ctr->serviceList();
   for(int i=0;i<serviceList.length();i++)
   {
       Service *service = serviceList[i];

       QPushButton *btn = new QPushButton();
       btn->setProperty("forUse","tabBtn");
       btn->setCursor(Qt::PointingHandCursor);
       btn->setCheckable(true);
       btn->setText(service->title());
       m_tabLayout->addWidget(btn);
       m_btnGroup->addButton(btn,i+1);
   }

   m_text = new QTextEdit(this);
   m_text->setReadOnly(true);

   connect(m_btnGroup,SIGNAL(buttonClicked(int)),this,SLOT(serviceClicked(int)));

   m_btnGroup->setExclusive(true);
}

void LogContainer::adjustLog()
{
    QTextDocument *doc =  m_text->document();
    QTextCursor textcursor = m_text->textCursor();
    for(QTextBlock it = doc->begin(); it !=doc->end();it = it.next())
    {
        QTextBlockFormat tbf = it.blockFormat();
        tbf.setLineHeight(10,QTextBlockFormat::LineDistanceHeight);
        textcursor.setPosition(it.position());
        textcursor.setBlockFormat(tbf);
        m_text->setTextCursor(textcursor);
    }
}

void LogContainer::serviceClicked(int index)
{
    m_tabIndex = index;

    if(index == 0)
        showSysLog();
    else
    {
        QList<Service*> serviceList = m_ctr->serviceList();
        Service *service = serviceList[index-1];
        ShowServiceLog(service);
    }
}

void LogContainer::addSeperator()
{
    QFrame *split = new QFrame();
    split->setProperty("forUse","vSplit");
    split->setObjectName("vSplit");
    split->setFrameShape(QFrame::VLine);
    m_tabLayout->addWidget(split);
}

void LogContainer::showSysLog()
{
    m_text->clear();
    foreach(MsgResult msg,m_sysLogResults)
    {
        m_text->append(transText(msg));
        if(msg.directOutput.isEmpty() == false)
            m_text->append(msg.directOutput);
    }

    adjustLog();
}

void LogContainer::ShowServiceLog(Service *service)
{
    m_text->clear();
    foreach(MsgResult msg,m_sysLogResults)
    {
        if(service->isFamilyType(msg.type))
        {
            m_text->append(transText(msg));
            if(msg.directOutput.isEmpty() == false)
                m_text->append(msg.directOutput);
        }
    }

    adjustLog();
}

void LogContainer::adjustAfterChanged()
{
    m_btnLog->setText(tlng("window.startLog"));

    m_tabWidget->setMinimumWidth(0);

    location();

//    QList<QAbstractButton*> btns = m_btnGroup->buttons();
//    foreach(QAbstractButton *btn,btns)
//    {
//        btn->adjustSize();
//    }

    m_tabWidget->adjustSize();

    QSize mainSize = size();
    QSize tabSize = m_tabWidget->size();

    int logMarginLeft = mainSize.height() * m_overRatio + ts(40);
    int marginTop = ts(20);
    int marginRight = ts(30);
    int marginBottom = ts(10);

    if(tabSize.width() <= mainSize.width() - logMarginLeft - marginRight)
        m_tabWidget->move(logMarginLeft,marginTop);
    else
        m_tabWidget->move(mainSize.width()-marginRight-tabSize.width(),marginTop);

    m_tabWidget->setMinimumWidth(mainSize.width() - m_tabWidget->x() - marginRight);

    int gap = ts(10);
    int logWidth = mainSize.width() - logMarginLeft - marginRight;
    int logHeight = mainSize.height() - marginTop - tabSize.height() - gap - marginBottom;

    m_text->setMinimumSize(logWidth,logHeight);
    m_text->setMaximumSize(logWidth,logHeight);

    m_text->move(logMarginLeft,marginTop+tabSize.height()+gap);



    if(m_tabIndex == 0)
        showSysLog();
    else
    {
        QList<Service*> serviceList = m_ctr->serviceList();
        Service *service = serviceList[m_tabIndex-1];

        ShowServiceLog(service);
    }

    adjustLog();
}

QString LogContainer::transText(MsgResult result)
{
//    QString msgStr = tlng(result.output);
//    msgStr.replace("${0}",result.placeholder1);
//    msgStr.replace("${1}",result.placeholder2);

//    return msgStr;

    return tlng(result);
}

void LogContainer::appendMsg(MsgResult msg)
{
    m_sysLogResults.append(msg);

    if(m_tabIndex == 0)
    {
        m_text->append(transText(msg));
        if(msg.directOutput.isEmpty() == false)
            m_text->append(msg.directOutput);
    }
    else
    {
        QList<Service*> serviceList = m_ctr->serviceList();
        Service *service = serviceList[m_tabIndex-1];

        if(service->isFamilyType(msg.type))
        {
             m_text->append(transText(msg));
             if(msg.directOutput.isEmpty() == false)
                 m_text->append(msg.directOutput);
        }
    }

    adjustLog();
}

void LogContainer::appendMsg(QString msg)
{
    MsgResult result;
    result.error = false;
    result.output = msg;

    appendMsg(result);
}

void LogContainer::clearMsgs()
{
    m_sysLogResults.clear();
    m_text->clear();
}
