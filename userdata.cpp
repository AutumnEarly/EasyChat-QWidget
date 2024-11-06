#include "userdata.h"

const int PIC_MAX_HEIGHT = 50;
const int PIC_MAX_WIDTH = 50;

// 用户基本信息
UserInfo::UserInfo(int uid, QString name, QString icon) :
    _uid(uid), _name(name), _nick(_name), _icon(icon), _sex(0)
{
    QUrl imgUrl(_icon);

    if(imgUrl.isLocalFile()) {
        qDebug() << "userInfo icon url: " << imgUrl.toLocalFile();
        QImage img(imgUrl.toLocalFile());
        img = img.scaledToHeight(PIC_MAX_HEIGHT, Qt::SmoothTransformation);
        _iconPixmap = QPixmap::fromImage(img);
    } else {
        qDebug() << "userInfo icon url: " << _icon;
        QImage img(_icon);
        img = img.scaledToHeight(PIC_MAX_HEIGHT, Qt::SmoothTransformation);
        _iconPixmap = QPixmap::fromImage(img);
    }

}
UserInfo::UserInfo(int uid, QString name, QString nick, QString icon, int sex) :
    _uid(uid),_name(name),_nick(nick),_icon(icon),_sex(sex)
{
    QUrl imgUrl(_icon);

    if(imgUrl.isLocalFile()) {
        qDebug() << "userInfo icon url: " << imgUrl.toLocalFile();
        QImage img(imgUrl.toLocalFile());
        img = img.scaledToHeight(PIC_MAX_HEIGHT, Qt::SmoothTransformation);
        _iconPixmap = QPixmap::fromImage(img);
    } else {
        qDebug() << "userInfo icon url: " << _icon;
        QImage img(_icon);
        img = img.scaledToHeight(PIC_MAX_HEIGHT, Qt::SmoothTransformation);
        _iconPixmap = QPixmap::fromImage(img);
    }

}
UserInfo::UserInfo(std::shared_ptr<FriendInfo> info) :
    _uid(info->m_uid), _name(info->m_name), _nick(_name), _icon(info->m_icon), _sex(info->m_sex)
{
    QUrl imgUrl(_icon);

    _iconPixmap = info->m_iconPixmap;

}
// 添加好友信息
AddFriendApply::AddFriendApply(int from_uid, QString name, QString desc, QString icon, QString nick, int sex)
{

}
// 申请好友信息
FriendApplyInfo::FriendApplyInfo(int uid, QString name, QString desc, QString icon, QString nick, int sex, int status)        :_uid(uid),_name(name),_desc(desc),
    _icon(icon),_nick(nick),_sex(sex),_status(status)
{

}

FriendApplyInfo::FriendApplyInfo(std::shared_ptr<AddFriendApply> addinfo) :
    _uid(addinfo->_from_uid),_name(addinfo->_name),_desc(addinfo->_desc),
    _icon(addinfo->_icon), _nick(addinfo->_nick), _sex(addinfo->_sex), _status(0)
{

}

FriendApplyInfo::FriendApplyInfo(int uid, QString name, QString icon, QString backmsg, int sex) :
    _uid(uid), _name(name), _icon(icon) ,_back_msg(backmsg), _sex(sex)
{

}

// 同意好友信息
FriendAuthInfo::FriendAuthInfo(int uid, QString name, QString nick, QString icon, int sex) :
    _uid(uid), _name(name), _nick(nick), _icon(icon),_sex(sex)
{

}

// 同意好友的回包信息
FriendAuthRsp::FriendAuthRsp(int peer_uid, QString peer_name, QString peer_nick,
    QString peer_icon, int peer_sex) : _uid(peer_uid), _name(peer_name), _nick(peer_nick),
    _icon(peer_icon), _sex(peer_sex)
{

}

// 好友信息
FriendInfo::FriendInfo(QObject* parent) : QObject(parent)
{

}

FriendInfo::~FriendInfo()
{

}

FriendInfo::FriendInfo(int uid, QString name, QString nick, QString icon, int sex,
                       QString desc, QString back, QString new_msg) :
    m_uid(uid), m_name(name), m_nick(nick), m_icon(icon), m_sex(sex),
    m_desc(desc), m_back(back), m_newMsg(new_msg)
{

    QUrl imgUrl(m_icon);

    if(imgUrl.isLocalFile()) {
        qDebug() << "icon url: " << imgUrl.toLocalFile();
        QPixmap img(imgUrl.toLocalFile());
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    } else {
        qDebug() << "icon url: " << m_icon;
        QPixmap img(m_icon);
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    }
}
FriendInfo::FriendInfo(std::shared_ptr<FriendAuthInfo> auth_info) : m_uid(auth_info->_uid),
    m_name(auth_info->_name), m_nick(auth_info->_nick), m_icon(auth_info->_icon),
    m_sex(auth_info->_sex)
{

    QUrl imgUrl(m_icon);

    if(imgUrl.isLocalFile()) {
        qDebug() << "icon url: " << imgUrl.toLocalFile();
        QPixmap img(imgUrl.toLocalFile());
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    } else {
        qDebug() << "icon url: " << m_icon;
        QPixmap img(m_icon);
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    }
}
FriendInfo::FriendInfo(std::shared_ptr<FriendAuthRsp> auth_rsp): m_uid(auth_rsp->_uid),
    m_name(auth_rsp->_name), m_nick(auth_rsp->_nick), m_icon(auth_rsp->_icon),
    m_sex(auth_rsp->_sex)
{

    QUrl imgUrl(m_icon);

    if(imgUrl.isLocalFile()) {
        qDebug() << "icon url: " << imgUrl.toLocalFile();
        QPixmap img(imgUrl.toLocalFile());
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    } else {
        qDebug() << "icon url: " << m_icon;
        QPixmap img(m_icon);
        img = img.scaled(PIC_MAX_WIDTH,PIC_MAX_HEIGHT,Qt::IgnoreAspectRatio ,Qt::SmoothTransformation);
        m_iconPixmap = img;
    }
}

FriendInfo::FriendInfo(const FriendInfo &info)
{
    m_uid = info.m_uid;
    m_nick = info.m_nick;
    m_icon = info.m_icon;
    m_iconPixmap = info.m_iconPixmap;
    m_back = info.m_back;
    m_name = info.m_name;
    m_newMsg = info.newMsg();

}


void FriendInfo::AppendChatMsg(std::shared_ptr<ChatMsgData> msg)
{
    this->m_chat_msg.append(msg);
    this->setNewMsg(msg->_msg_content);
    this->setNewTime(msg->_dateTime);
}

void FriendInfo::prependChatMsg(std::shared_ptr<ChatMsgData> msg)
{
    this->m_chat_msg.prepend(msg);
}

void FriendInfo::UpdateChatMsgStatus(const QString& msgId,int status)
{
//    qFind(m_chat_msg.rbegin(),m_chat_msg.rend(),[=](std::shared_ptr<ChatMsgData> msg) {
//        if(msgId == msg->_msg_id) {
//            msg->_status = status;
//            return true;
//        }
//        return false;
//    });
}

QString FriendInfo::newMsg() const
{
    return m_newMsg;
}

void FriendInfo::setNewMsg(const QString &newNewMsg)
{
    if (m_newMsg == newNewMsg)
        return;
    m_newMsg = newNewMsg;
    emit newMsgChanged(m_newMsg);
}

QDateTime FriendInfo::newTime() const
{
    return m_newTime;
}

void FriendInfo::setNewTime(const QDateTime &newNewTime)
{
    if (m_newTime == newNewTime)
        return;
    m_newTime = newNewTime;
    emit newTimeChanged(m_newTime);
}

int FriendInfo::unreadMsg() const
{
    return m_unreadMsg;
}

void FriendInfo::setUnreadMsg(int newUnreadMsg)
{
    if (m_unreadMsg == newUnreadMsg)
        return;
    m_unreadMsg = newUnreadMsg;
    emit unreadMsgChanged(m_unreadMsg);
}

int FriendInfo::allChatMsgCount() const
{
    return m_allChatMsgCount;
}

void FriendInfo::setAllChatMsgCount(int newAllChatMsgCount)
{
    if (m_allChatMsgCount == newAllChatMsgCount)
        return;
    m_allChatMsgCount = newAllChatMsgCount;
    emit allChatMsgCountChanged();
}





