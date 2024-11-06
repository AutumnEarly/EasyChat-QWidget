#include "usermgr.h"

UserMgr::UserMgr() : m_userInfo(std::make_shared<UserInfo>(0,"Login","")) , m_status(0)
{
    connect(this,&UserMgr::statusChanged,this,[&]() {
        if(this->m_status == -1) {
            qDebug() << "设备已下线！";
        } else if(this->m_status == 0) {
            qDebug() << "设备已上线！";
        }
    });
}

UserMgr::~UserMgr() {

}

void UserMgr::initHandlers()
{

}

int UserMgr::status() const
{
    return m_status;
}

void UserMgr::setStatus(int newStatus)
{
    if (m_status == newStatus)
        return;
    m_status = newStatus;
    emit statusChanged();
}

void UserMgr::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    m_userInfo = userInfo;
}

void UserMgr::setToken(QString token)
{
    m_token = token;
}

std::shared_ptr<UserInfo> UserMgr::getUserInfo()
{
    return this->m_userInfo;
}

int UserMgr::getUid()
{
   return m_userInfo->_uid;
}

QString UserMgr::getName()
{
   return m_userInfo->_name;
}


void UserMgr::appendApplyList(const QJsonArray& array)
{
   for (const QJsonValue &value : array) {
       auto name = value["name"].toString();
       auto desc = value["desc"].toString();
       auto icon = value["icon"].toString();
       auto nick = value["nick"].toString();
       auto sex = value["sex"].toInt();
       auto uid = value["uid"].toInt();
       auto status = value["status"].toInt();
       auto info = std::make_shared<FriendApplyInfo>(uid, name,
                                               desc, icon, nick, sex, status);
       m_friendApply_list.push_back(info);
   }
}
void UserMgr::appendApplyList(std::shared_ptr<FriendApplyInfo> info)
{
   m_friendApply_list.push_back(info);
}
QVector<std::shared_ptr<FriendApplyInfo>> UserMgr::getApplyList()
{
   return m_friendApply_list;
}

void UserMgr::appendFriendList(QJsonArray array) {

   for (const QJsonValue& value : array) {
       auto name = value["name"].toString();
       auto desc = value["desc"].toString();
       auto icon = value["icon"].toString();
       auto nick = value["nick"].toString();
       auto sex = value["sex"].toInt();
       auto uid = value["uid"].toInt();
       auto back = value["back"].toString();

       auto info = std::make_shared<FriendInfo>(uid, name,
                                                nick, icon, sex, desc, back);
       m_firendInfo_list.push_back(info);
       m_friendInfo_map.insert(uid, info);
   }
}

void UserMgr::addFriendInfo(const QJsonObject &value)
{
   auto name = value["name"].toString();
   auto desc = value["desc"].toString();
   auto icon = value["icon"].toString();
   auto nick = value["nick"].toString();
   auto sex = value["sex"].toInt();
   auto uid = value["uid"].toInt();
   auto back = value["back"].toString();

   auto info = std::make_shared<FriendInfo>(uid, name,
                                            nick, icon, sex, desc, back);
   m_firendInfo_list.push_back(info);
   m_friendInfo_map.insert(uid, info);
}

std::shared_ptr<FriendInfo> UserMgr::getFriendInfoById(int id) {
   if(m_friendInfo_map.find(id) == m_friendInfo_map.end()) {
       qDebug() << "not find friendInfo id is" << id;
       return nullptr;
   }
   return m_friendInfo_map.value(id);
}
QMap<int, std::shared_ptr<FriendInfo>> UserMgr::getFriendInfo()
{
   return this->m_friendInfo_map;
}

QVector<std::shared_ptr<ChatMsgData>> UserMgr::getFriendChatMsg(int uid)
{
    if(m_friendInfo_map.find(uid) == m_friendInfo_map.end()) {
       return QVector<std::shared_ptr<ChatMsgData>>();
    }
    return m_friendInfo_map.value(uid)->m_chat_msg;
}

void UserMgr::appendFriendChatMsg(int uid, std::shared_ptr<ChatMsgData> msgdata)
{
    auto it = m_friendInfo_map.find(uid);
    if(it == m_friendInfo_map.end()) {
       return;
    }

    it.value()->AppendChatMsg(msgdata);
}

void UserMgr::setFriendNewChatMsg(int uid, QString new_msg)
{
    auto it = m_friendInfo_map.find(uid);
    if(it == m_friendInfo_map.end()) {
       return;
    }
    it.value()->setNewMsg(new_msg);
}

