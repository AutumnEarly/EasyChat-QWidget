#include "chatlist.h"
#include <QDebug>
#include <QList>
#include <QEvent>
#include <QWheelEvent>
#include <QHBoxLayout>
#include <QThread>
#include "chatlistitem.h"
#include "usermgr.h"
#include "tcpmgr.h"
#include <QQueue>

ChatList::ChatList(QWidget * parent) : QListWidget(parent)
{
//    this->horizontalScrollBar()->hide();
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    // 安装事件过滤器
    this->viewport()->installEventFilter(this);

    auto data = UserMgr::GetInstance()->getFriendInfo();
    this->m_count = data.count();
    this->m_loadCount = 0;

    int i = 0;

    for(const auto & friendInfo : data) {
        m_loadCount++;
        qDebug() << "loadCount " << m_loadCount <<"fiendInfo name: " << friendInfo->m_name << " id " << friendInfo->m_uid << "newMsg " << friendInfo->newMsg();
        QListWidgetItem* item = new QListWidgetItem();
        ChatListItem* con_user_item = new ChatListItem( );

        item->setSizeHint(QSize(110,80));

        con_user_item->setInfo(friendInfo->m_name,friendInfo->newMsg(),
                               friendInfo->newTime(),1,friendInfo->m_iconPixmap);
        con_user_item->setFriendInfo(friendInfo);
        con_user_item->setUserInfo(std::make_shared<UserInfo>(friendInfo));
        this->addItem(item);
        this->setItemWidget(item, con_user_item);

//        qDebug() << "con_uuser: " << con_user_item->parentWidget() << " " <<con_user_item->parent();

        QJsonObject obj;
        QDateTime startTime = QDateTime::currentDateTime().toLocalTime();

        obj["uid"] = friendInfo->m_uid;
        obj["device"] = this_device_name;
        obj["startTime"] = startTime.toString("yyyy-MM-dd hh:mm:ss");
        obj["limit"] = 30;

        m_cacheData.enqueue(std::make_shared<QJsonObject>(obj));
    }

    connect(UserMgr::GetInstance().get(),&UserMgr::sig_addFriend,this,[&](std::shared_ptr<FriendInfo> info) {
        appendItem(info);
    });

    // TODO: 由于目前是聊天服务器直接发送的聊天记录，
    // 如果同一时间发送请求会导致服务器返回不及时，然后客户端假卡死

    connect(&m_timer,&QTimer::timeout,[=](){
        if(!m_cacheData.empty()) {
            // 发送获取聊天记录的请求
            emit TcpMgr::GetInstance()->sig_send_data(
                ReqId::ID_GET_CHAT_MSG_REQ,QJsonDocument(*m_cacheData.dequeue()).toJson()
            );
            m_timer.setInterval(50);
            m_timer.start();
            return;
        }
    });
    m_timer.setInterval(50);
    m_timer.start();
}

int ChatList::swapItemPos(int index1, int index2)
{

    if(index1 < 0 || index2 < 0) {
        return -1;
    }

    QWidget* item1 = this->itemWidget(this->item(index1));
    if(!item1) {
        qDebug() << "chatlistitem error !"<< item1;
        return -1;
    }
    ChatListItem* chatItem1 = qobject_cast<ChatListItem*>(item1);
    if(!chatItem1) {
        qDebug() << "chatlistitem error !" << chatItem1;
        return -1;
    }
    QWidget* item2 = this->itemWidget(this->item(index2));
    if(!item1) {
        qDebug() << "chatlistitem error !"<< item2;
        return -1;
    }
    ChatListItem* chatItem2 = qobject_cast<ChatListItem*>(item2);
    if(!chatItem1 || chatItem2 == chatItem1) {
        qDebug() << "chatlistitem error !" << chatItem2;
        return -1;
    }

    if(this->currentItem() == this->item(index1)) {
        this->setCurrentItem(this->item(index2));
    } else if(this->currentItem() == this->item(index2)) {
        this->setCurrentItem(this->item(index1));
    }
    // 更新信息和当前选择的项
    chatItem1->swapInfo(chatItem2);
//    this->setCurrentItem(this->item(index2));
    return 0;
}

ChatListItem* ChatList::findItem(std::shared_ptr<UserInfo> userInfo)
{
    ChatListItem* item = nullptr;
    for(int i = 0; i < this->count();i++) {
        // 1.查询推送的消息，在聊天栏是否存在
        QWidget* item = this->itemWidget(this->item(i));
        if(!item) {
            qDebug() << "chatlistitem error !"<< item;
            continue;
        }
        ChatListItem* chatItem = qobject_cast<ChatListItem*>(item);
        if(!chatItem) {
            qDebug() << "chatlistitem error !" << chatItem;
            continue;
        }
        std::shared_ptr<UserInfo> itemInfo = chatItem->getUserInfo();
        if(itemInfo->_uid == userInfo->_uid) {
            item = chatItem;
            break;
        }
    }

    return item;
}

int ChatList::findItemIndex(std::shared_ptr<UserInfo> userInfo)
{
    int index = -1;
    for(int i = 0; i < this->count();i++) {
        // 1.查询推送的消息，在聊天栏是否存在
        QWidget* item = this->itemWidget(this->item(i));
        if(!item) {
            qDebug() << "chatlistitem error !"<< item;
            continue;
        }
        ChatListItem* chatItem = qobject_cast<ChatListItem*>(item);
        if(!chatItem) {
            qDebug() << "chatlistitem error !" << chatItem;
            continue;
        }
        std::shared_ptr<UserInfo> itemInfo = chatItem->getUserInfo();
        if(itemInfo->_uid == userInfo->_uid) {
            index = i;
            break;
        }
    }

    return index;
}

void ChatList::appendItem(std::shared_ptr<FriendInfo> friendInfo)
{
    QListWidgetItem* item = new QListWidgetItem();
    ChatListItem* con_user_item = new ChatListItem(this);

    item->setSizeHint(QSize(110,80));
    this->addItem(item);

    if(friendInfo->m_iconPixmap.isNull()) {
        QPixmap pixmap(friendInfo->m_icon);
        friendInfo->m_iconPixmap = pixmap.scaled(50,50,Qt::IgnoreAspectRatio,Qt::SmoothTransformation);
    }
    con_user_item->setFriendInfo(friendInfo);
    con_user_item->setInfo(friendInfo->m_name,friendInfo->newMsg(),friendInfo->newTime(),1,friendInfo->m_iconPixmap);

    con_user_item->setUserInfo(std::make_shared<UserInfo>(friendInfo));
    this->setItemWidget(item,con_user_item);
}

void ChatList::removeItem(std::shared_ptr<UserInfo> userInfo)
{
    for(int i = 0; i < this->count();i++) {
        // 1.查询推送的消息，在聊天栏是否存在
        QWidget* item = this->itemWidget(this->item(i));
        if(!item) {
            qDebug() << "chatlistitem error !"<< item;
            continue;
        }
        ChatListItem* chatItem = qobject_cast<ChatListItem*>(item);
        if(!chatItem) {
            qDebug() << "chatlistitem error !" << chatItem;
            continue;
        }
        std::shared_ptr<UserInfo> itemInfo = chatItem->getUserInfo();
        if(itemInfo->_uid == userInfo->_uid) {
            delete QListWidget::takeItem(i);
            return;
        }
    }
}

bool ChatList::eventFilter(QObject *watched, QEvent *event)
{
    if(watched == this->viewport()) {
        if(event->type() == QEvent::Enter) {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        } else if(event->type() == QEvent::Leave) {
            this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    // 检查事件是否是鼠标滚轮事件
    if (watched == this->viewport() && event->type() == QEvent::Wheel) {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y() / 8;
        int numSteps = numDegrees / 15; // 计算滚动步数

        // 设置滚动幅度
        this->verticalScrollBar()->setValue(this->verticalScrollBar()->value() - numSteps);

        // 检查是否滚动到底部
        QScrollBar *scrollBar = this->verticalScrollBar();
        int maxScrollValue = scrollBar->maximum();
        int currentValue = scrollBar->value();
        //int pageSize = 10; // 每页加载的联系人数量

        if (maxScrollValue - currentValue <= 0) {

            //发送信号通知聊天界面加载更多聊天内容

        }

        return true; // 停止事件传递
    }

    return QListWidget::eventFilter(watched,event);
}

void ChatList::slot_chatItemInfoChanged(ChatListItem *item)
{

}


