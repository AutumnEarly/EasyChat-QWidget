#include "chatlistitem.h"
#include "ui_chatlistitem.h"

#include "qctool.h"

ChatListItem::ChatListItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatListItem)
{
    ui->setupUi(this);

}

ChatListItem::~ChatListItem()
{
    qDebug() << "chatListItem ~" << this;
    delete ui;
}

void ChatListItem::setInfo(const QString& name, const QString& new_msg,
    const QDateTime& new_time, const int unread_msg_cnt, const QPixmap& icon)
{

    this->setName(name);
    this->setNewMsg(new_msg);
    this->setNewTime(new_time);
    this->setUnreadMsgCnt(unread_msg_cnt);
    this->setIcon(QCTool::RoundImage(icon,50,50,5));

}

void ChatListItem::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    this->m_userInfo = userInfo;
}

void ChatListItem::swapInfo(ChatListItem *item)
{
    QString name = m_name;
    QString newMsg = m_newMsg;
    QDateTime newTime = m_newTime;
    int unreadMsgCnt = m_unreadMsgCnt;
    QPixmap icon = m_icon;
    std::shared_ptr<UserInfo> userInfo = m_userInfo;
    std::shared_ptr<FriendInfo> friendInfo = m_friendInfo;

    this->setName(item->name());
    this->setNewMsg(item->newMsg());
    this->setNewTime(item->newTime());
    this->setUnreadMsgCnt(item->unreadMsgCnt());
    this->setIcon(item->icon());
    this->setUserInfo(item->getUserInfo());
    this->setFriendInfo(item->friendInfo());

    item->setName(name);
    item->setNewMsg(newMsg);
    item->setNewTime(newTime);
    item->setUnreadMsgCnt(unreadMsgCnt);
    item->setIcon(icon);
    item->setUserInfo(userInfo);
    item->setFriendInfo(friendInfo);

}



std::shared_ptr<UserInfo> ChatListItem::getUserInfo()
{
    return this->m_userInfo;
}

QString ChatListItem::name() const
{
    return m_name;
}

void ChatListItem::setName(const QString &newName)
{
    if (m_name == newName)
        return;

    m_name = newName;
    ui->user_name_lb->setText(m_name);
    emit nameChanged();
}

QString ChatListItem::newMsg() const
{
    return m_newMsg;
}

void ChatListItem::setNewMsg(const QString &newNewMsg)
{
    if (m_newMsg == newNewMsg)
        return;

    m_newMsg = newNewMsg;
    ui->new_msg_lb->setText(m_newMsg);
    emit newMsgChanged();
}

QDateTime ChatListItem::newTime() const
{
    return m_newTime;
}

void ChatListItem::setNewTime(const QDateTime &newNewTime)
{
    if (m_newTime == newNewTime)
        return;

    m_newTime = newNewTime;
    QString dateTime = "";
    QString dateFormat = "yyyy-MM-dd";
    if(QDateTime::currentDateTime().date().day() - newNewTime.date().day() <= 0) {
        dateTime = m_newTime.time().toString("hh:mm");
    } else {
        dateTime = m_newTime.date().toString("MM dd");
    }

    ui->new_time_msg_lb->setText(dateTime);
    qDebug() << "msg new time: " << dateTime;
    emit newTimeChanged(this, m_newTime);
}

int ChatListItem::unreadMsgCnt() const
{
    return m_unreadMsgCnt;
}

void ChatListItem::setUnreadMsgCnt(int newUnreadMsgCnt)
{
    if (m_unreadMsgCnt == newUnreadMsgCnt)
        return;

    m_unreadMsgCnt = newUnreadMsgCnt;
    ui->unread_msg_cnt_lb->setText(QString::number(m_unreadMsgCnt));
    emit unreadMsgCntChanged();
}

QPixmap ChatListItem::icon() const
{
    return m_icon;
}


void ChatListItem::setIcon(const QPixmap &newIcon)
{
    if (m_icon == newIcon)
        return;

    m_icon = newIcon;

    ui->head_portrait_lb->setPixmap(m_icon);
    emit iconChanged();
}

std::shared_ptr<FriendInfo> ChatListItem::friendInfo() const
{
    return m_friendInfo;
}

void ChatListItem::setFriendInfo(const std::shared_ptr<FriendInfo> &newFriendInfo)
{
    if (m_friendInfo.get() == newFriendInfo.get())
        return;

    if(m_friendInfo) {
        disconnect(m_friendInfo.get(),&FriendInfo::newMsgChanged,this,&ChatListItem::setNewMsg);
        disconnect(m_friendInfo.get(),&FriendInfo::newTimeChanged,this,&ChatListItem::setNewTime);
        disconnect(m_friendInfo.get(),&FriendInfo::unreadMsgChanged,this,&ChatListItem::setUnreadMsgCnt);
    }

    m_friendInfo = newFriendInfo;
    connect(m_friendInfo.get(),&FriendInfo::newMsgChanged,this,&ChatListItem::setNewMsg,Qt::UniqueConnection);
    connect(m_friendInfo.get(),&FriendInfo::newTimeChanged,this,&ChatListItem::setNewTime,Qt::UniqueConnection);
    connect(m_friendInfo.get(),&FriendInfo::unreadMsgChanged,this,&ChatListItem::setUnreadMsgCnt,Qt::UniqueConnection);

    emit friendInfoChanged();
}


