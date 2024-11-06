#include "chattingrecordslist.h"
#include <QLabel>
#include <QRandomGenerator>
#include <QMouseEvent>
#include <QWheelEvent>
#include "../msgBubble/TextBubble.h"
#include "../msgBubble/pictureBubble.h"
#include "../qctool.h"
#include "../tcpmgr.h"

ChattingRecordsList::ChattingRecordsList(QWidget *parent)
    : QWidget(parent) , m_isAppended(false), m_maxLoadCount(10)
{

    // 1. 创建一个水平布局
    QVBoxLayout *pMainLayout = new QVBoxLayout();
    this->setLayout(pMainLayout);
    pMainLayout->setMargin(0);

    // 2. 创建一个滚动区域
    m_scrollArea = new QScrollArea();
    m_scrollArea->setObjectName("chat_area");
    pMainLayout->addWidget(m_scrollArea);

    // 3. 基于滚动区域的背景
    QWidget *w = new QWidget(this);
    w->setObjectName("chat_records_bg");
    w->setAutoFillBackground(true);

    // 4. 用于存放消息的布局
    QVBoxLayout *pVLayout_1 = new QVBoxLayout();
    pVLayout_1->addWidget(new QWidget(), 100000);
    // 设置每条消息的上下间隔
    pVLayout_1->setSpacing(15);
    w->setLayout(pVLayout_1);

    m_scrollArea->setWidget(w);
    m_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QScrollBar *pVScrollBar = m_scrollArea->verticalScrollBar();
    connect(pVScrollBar,&QScrollBar::rangeChanged,this,&ChattingRecordsList::onVScroolBatMoved);

    connect(pVScrollBar,&QScrollBar::valueChanged,this,[=](int val) {
//        qDebug() << "value " << val;
    });

    //把垂直ScrollBar放到上边 而不是原来的并排
    QHBoxLayout *pHLayout_2 = new QHBoxLayout();
    pHLayout_2->addWidget(pVScrollBar, 0, Qt::AlignRight);
    pHLayout_2->setMargin(0);

    // 设置滚动区域的左右间隔
    pHLayout_2->setSpacing(20);
    m_scrollArea->setLayout(pHLayout_2);
    pVScrollBar->setHidden(true);


    m_vScroolBar = m_scrollArea->verticalScrollBar();

    m_scrollArea->setWidgetResizable(true);
    m_scrollArea->installEventFilter(this);

    connect(this,&ChattingRecordsList::sig_loadItem,this,&ChattingRecordsList::slot_loadItem);
    connect(this,&ChattingRecordsList::sig_loadItem_accomplish,
            this,&ChattingRecordsList::slot_loadItem_accomplish);

}

ChattingRecordsList::~ChattingRecordsList()
{
    qDebug() << "ChattingRecordsList delete!";
}

void ChattingRecordsList::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    connect(UserMgr::GetInstance().get(),&UserMgr::sig_appendedFriendChatMsg,
            this,&ChattingRecordsList::slot_appendedFriendMsg,Qt::UniqueConnection);

    m_userInfo = userInfo;
    qDebug() << m_userInfo->_name;

    QVector<std::shared_ptr<ChatMsgData>> data = UserMgr::GetInstance()->getFriendChatMsg(m_userInfo->_uid);

    auto selfInfo = UserMgr::GetInstance()->getUserInfo();

    int i = 0;
    int count = data.count() - 1;
    for(int i = 0; i < m_maxLoadCount && i <= count;i++) {
        if(data[count - i]->_from_uid == m_userInfo->_uid) {

            this->prependChatMsg(m_userInfo, data[count - i]);
        } else {

            this->prependChatMsg(selfInfo, data[count - i]);
        }
//        qDebug() << "dateTime: " << m_data.first().second->_dateTime;

    }
    m_count = m_data.count();

    for(i = count - m_maxLoadCount;i >= 0;--i) {

        if(data[i]->_from_uid == m_userInfo->_uid) {
            m_headCache.enqueue(QPair(m_userInfo, data[i]));
        } else {
            m_headCache.enqueue(QPair(selfInfo, data[i]));
        }
    }
}

ChattingRecordsList::shared_ptr<UserInfo> ChattingRecordsList::getUserInfo()
{
    return m_userInfo;
}

void ChattingRecordsList::appendChatMsg(std::shared_ptr<UserInfo> userInfo,
    std::shared_ptr<ChatMsgData> msgData, bool isAdd)
{
    if(isAdd) {
        this->appendChatItem(userInfo, msgData);

        m_data.push_back(QPair(userInfo, msgData));
    } else {
        m_tailCache.enqueue(QPair(userInfo, msgData));
    }

}

void ChattingRecordsList::prependChatMsg(std::shared_ptr<UserInfo> userInfo,
    std::shared_ptr<ChatMsgData> msgData, bool isAdd)
{
    if(isAdd) {

        this->prependChatItem(userInfo, msgData);

        m_data.prepend(QPair(userInfo, msgData));
    } else {
        m_headCache.enqueue(QPair(userInfo, msgData));
    }
}



void ChattingRecordsList::appendChatItem(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData)
{
    int count = m_data.count();

    if(count >= 0) {
        int minute1 = msgData->_dateTime.time().minute();
        int minute2 = m_data[count-1].second->_dateTime.time().minute();
        int difference = minute1 - minute2;

        qDebug() << "appendChatItem difference: " << difference << " " << msgData->_dateTime.time()
                 << " " << m_data[count-1].second->_dateTime.time();
        if(difference > 2) {
            this->addTipItem(count-1,msgData->_dateTime.time().toString("mm:ss"));
        }

    }

    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_scrollArea->widget()->layout());
    ChatMsgRole msgRole = ChatMsgRole::Self;
    if(msgData->_from_uid != UserMgr::GetInstance()->getUid()) {
        msgRole = ChatMsgRole::Other;
    }
    ChatMsgItemBase *msgItem = new ChatMsgItemBase(nullptr,msgRole);
    if(m_usersHeadImg.find(userInfo->_uid) == m_usersHeadImg.end()) {
        m_usersHeadImg[userInfo->_uid] = QCTool::RoundImage(userInfo->_iconPixmap,
                                                            50,50,8);
    }
    msgItem->setUserIcon(m_usersHeadImg[userInfo->_uid]);
    if(msgRole == Self) {
        msgItem->setUserName(msgData->_dateTime.toString("yyyy/MM/dd hh:mm:ss") + " 日期:" + userInfo->_name);
    } else {
        msgItem->setUserName(userInfo->_name + " " + msgData->_dateTime.toString("yyyy/MM/dd hh:mm:ss"));
    }


    if(msgData->_type == "text") {
        TextBubble* tbubble = new TextBubble(msgRole,msgData->_msg_content);
        msgItem->setWidget(tbubble);

    } else if(msgData->_type == "image") {
        PictureBubble* pBubble = new PictureBubble(msgRole,msgData->_msg_content);
        msgItem->setWidget(pBubble);
    }
    vl->insertWidget(vl->count()-1, msgItem);

}

void ChattingRecordsList::prependChatItem(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData)
{

    int count = m_data.count();

    if(count > 0) {
        int minute1 = m_data[0].second->_dateTime.time().minute();
        int minute2 = msgData->_dateTime.time().minute();
        int difference = minute1 - minute2;

        qDebug() << "prependChatItem difference: " << difference << " " << msgData->_dateTime.time()
                 << " " << m_data[0].second->_dateTime.time();

        if(difference > 2) {
            this->addTipItem(0,msgData->_dateTime.time().toString("mm:ss"));
        }
    }

    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_scrollArea->widget()->layout());
    ChatMsgRole msgRole = ChatMsgRole::Self;
    if(msgData->_from_uid != UserMgr::GetInstance()->getUid()) {
        msgRole = ChatMsgRole::Other;
    }
    ChatMsgItemBase *msgItem = new ChatMsgItemBase(nullptr,msgRole);

    if(m_usersHeadImg.find(userInfo->_uid) == m_usersHeadImg.end()) {
        m_usersHeadImg[userInfo->_uid] = QCTool::RoundImage(userInfo->_iconPixmap,
                                                            50,50,8);
    }
    msgItem->setUserIcon(m_usersHeadImg[userInfo->_uid]);

    if(msgRole == Self) {
        msgItem->setUserName(msgData->_dateTime.toString("yyyy/MM/dd hh:mm:ss") + " " + userInfo->_name);
    } else {
        msgItem->setUserName(userInfo->_name + " " + msgData->_dateTime.toString("yyyy/MM/dd hh:mm:ss"));
    }

    if(msgData->_type == "text") {
        TextBubble* tbubble = new TextBubble(msgRole,msgData->_msg_content);
        msgItem->setWidget(tbubble);

    } else if(msgData->_type == "image") {
        PictureBubble* pBubble = new PictureBubble(msgRole,msgData->_msg_content);
        msgItem->setWidget(pBubble);
    }
    vl->insertWidget(0, msgItem);


}

void ChattingRecordsList::insertChatItem(QWidget *item, QWidget *beforeItem)
{

}

void ChattingRecordsList::clearChatItem()
{
    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_scrollArea->widget()->layout());
    int count = vl->count();
    for (int i = 0; i < count - 1; ++i) {
        QLayoutItem *item = vl->takeAt(0); // 始终从第一个控件开始删除
        if (item) {
            if (QWidget *widget = item->widget()) {
                delete widget;
            }
            delete item;
        }
    }

}

void ChattingRecordsList::scrollToBottom()
{
    m_vScroolBar->setValue(m_vScroolBar->maximum());
}

void ChattingRecordsList::addTipItem(int index, const QString & tip)
{

    QVBoxLayout *vl = qobject_cast<QVBoxLayout *>(m_scrollArea->widget()->layout());

    QLabel* tipLabel = new QLabel(tip);
    tipLabel->setMaximumSize(40,25);
    vl->insertWidget(index, tipLabel);

    qDebug() << "insert Tip Item ~ " << tip;
}

void ChattingRecordsList::slot_loadItem(int direction)
{
    int loadCount = 0;
    int isNull = m_headCache.isEmpty();

    if(direction == 0) { // 上

        for(int i = 0; i < m_maxLoadCount && !m_headCache.isEmpty();i++) {

            auto cache = m_headCache.dequeue();
            m_data.prepend(cache);

            std::shared_ptr<UserInfo> userInfo = cache.first;
            std::shared_ptr<ChatMsgData> msgData = cache.second;
            this->prependChatItem(userInfo,msgData);
            ++loadCount;
        }

    } else if(direction == 1) { // 下
        for(int i = 0; i < m_maxLoadCount && !m_tailCache.isEmpty();i++) {

            auto cache = m_tailCache.dequeue();
            m_data.push_back(cache);

            std::shared_ptr<UserInfo> userInfo = cache.first;
            std::shared_ptr<ChatMsgData> msgData = cache.second;
            this->appendChatItem(userInfo,msgData);
            ++loadCount;
        }

    }
    m_count = m_data.count();
    if(!isNull) {
        emit sig_loadItem_accomplish();

        qDebug() << "load Chat MSG Count: " << loadCount;
    } else {
        if(m_data.count() >= UserMgr::GetInstance()->getFriendInfoById(m_userInfo->_uid)->allChatMsgCount()) {
            qDebug() << "There are no more historical records !";
            emit sig_loadItem_accomplish();
            return;
        }

        QJsonObject obj;
        QDateTime startTime = m_data.first().second->_dateTime;

        obj["uid"] = m_userInfo->_uid;
        obj["device"] = this_device_name;
        obj["startTime"] = startTime.toString("yyyy-MM-dd hh:mm:ss");
        obj["limit"] = 30;
        qDebug() << "GET recordsList dateTime: " << startTime;
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_GET_CHAT_MSG_REQ,QJsonDocument(obj).toJson());
    }

}

void ChattingRecordsList::slot_loadItem_accomplish()
{
    m_isAppended = false;
}

void ChattingRecordsList::slot_appendedFriendMsg(int uid, QVector<std::shared_ptr<ChatMsgData> > msgData)
{
    qDebug() << "slot_appendedFriendMsg uid: " << uid << " count: " << msgData.count();
    int dir = 1;
    auto selfInfo = UserMgr::GetInstance()->getUserInfo();

    // 为空就停止
    if(msgData.isEmpty()) {
        slot_loadItem(0);
        return;
    }

    // 判断是历史消息还是新消息
    if(msgData[0]->_dateTime < m_data[0].second->_dateTime) {
        dir = 0;
        foreach (auto msg, msgData) {
            if(msg->_from_uid == m_userInfo->_uid) {
                m_headCache.enqueue(QPair(m_userInfo, msg));
            } else {
                m_headCache.enqueue(QPair(selfInfo, msg));
            }
        }
    } else {
        foreach (auto msg, msgData) {
            if(msg->_from_uid == m_userInfo->_uid) {
                m_tailCache.enqueue(QPair(m_userInfo, msg));
            } else {
                m_tailCache.enqueue(QPair(selfInfo, msg));
            }
        }
    }

    slot_loadItem(dir);

}


void ChattingRecordsList::onVScroolBatMoved(int min, int max)
{
    // TODO: 实现发送消息自动下移(未完成)

    QScrollBar *scroolBar = m_scrollArea->verticalScrollBar();
    if(scroolBar->value() == scroolBar->maximum() - 1) {
        scroolBar->setValue(scroolBar->maximum());
    }
//    qDebug() << "value: " << scroolBar->value() << " max: " << max;
}

bool ChattingRecordsList::eventFilter(QObject *obj, QEvent *event)
{
    auto handleWheel = [=](QEvent* event) {
        QWheelEvent* e = static_cast<QWheelEvent*>(event);
        qDebug() << "value: " << m_vScroolBar->value() << " max: " << m_vScroolBar->maximum();

    };
    if(obj == m_scrollArea) {
        switch (event->type()) {
        case QEvent::Enter:
            m_scrollArea->verticalScrollBar()->setHidden(
                m_scrollArea->verticalScrollBar()->maximum() == 0);
            break;
        case QEvent::Leave:
            m_scrollArea->verticalScrollBar()->setHidden(true);
            break;
        case QEvent::Wheel:
            handleWheel(event);
            break;
        default:
            break;
        }
    }
    return QWidget::eventFilter(obj, event);
}

void ChattingRecordsList::paintEvent(QPaintEvent *event)
{
    // 绘制时初始化样式
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

}

void ChattingRecordsList::wheelEvent(QWheelEvent *event)
{
    int numDegrees = event->angleDelta().y() / 8;
    int numSteps = numDegrees / 15; // 一次滚动的步数

    qDebug() << "value: " << m_vScroolBar->value() << " max: " << m_vScroolBar->maximum();

    if(m_isAppended) {
        return;
    }

    // TODO: 通知chatPage加载数据 当前抖动过大未消抖
    QScrollBar* vBar = m_scrollArea->verticalScrollBar();
    if (numSteps > 0 && vBar->value() == 0)
    {
        m_isAppended = true;
        emit sig_loadItem(0);
        qDebug() << "Mouse wheel scrolled up.";
    }
    else if (numSteps < 0)
    {
        m_isAppended = true;
        emit sig_loadItem(1);
        qDebug() << "Mouse wheel scrolled down.";
    }
}

void ChattingRecordsList::resizeEvent(QResizeEvent *event)
{
    // 获取新的宽度和高度
    int newWidth = event->size().width();
    int newHeight = event->size().height();

//    qDebug() << "Widget resized. New width: " << newWidth << ", new height: " << newHeight;
//    this->scrollToBottom();
}

void ChattingRecordsList::initStyleSheet()
{
    QFile file(":/style/chattingRecordsView.qss");
    if(file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    }
    else {
        qDebug() << "文件打开失败!";
    }
}
