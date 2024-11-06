#ifndef USERMGR_H
#define USERMGR_H

#include <QObject>
#include "singleton.h"
#include "global.h"
#include "userdata.h"

/*********************************************************************************
  * @Version:
  * @FileName:      usermgr.h
  * @Brief:         管理当前登录的用户信息

  * @Author:        秋初
  * @Date:          2024-09-15
  * @History:
**********************************************************************************/

class UserMgr : public QObject, public Singleton<UserMgr>,
                public std::enable_shared_from_this<UserMgr>

{
    Q_OBJECT
    friend class Singleton<UserMgr>;
public:
    ~UserMgr();
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);
    void setToken(QString token);
    std::shared_ptr<UserInfo> getUserInfo();
    int getUid();
    QString getName();

    void appendApplyList(const QJsonArray& array);
    void appendApplyList(std::shared_ptr<FriendApplyInfo> info);
    QVector<std::shared_ptr<FriendApplyInfo>> getApplyList();

    void appendFriendList(QJsonArray array);
    void addFriendInfo(const QJsonObject& obj);

    std::shared_ptr<FriendInfo> getFriendInfoById(int id);
    QMap<int, std::shared_ptr<FriendInfo>> getFriendInfo();
    QVector<std::shared_ptr<ChatMsgData>> getFriendChatMsg(int uid);

    void appendFriendChatMsg(int friendId, std::shared_ptr<ChatMsgData> msgdata);

    std::shared_ptr<ChatMsgData> getFriendNewMsg();

    void setFriendNewChatMsg(int uid, QString new_msg);
    int status() const;
    void setStatus(int newStatus);

signals:
    void sig_appendedFriendChatMsg(int uid, QVector<std::shared_ptr<ChatMsgData>> msgData);
    void sig_addFriendChatMsg();
    void sig_addFriend(std::shared_ptr<FriendInfo> info);
    void statusChanged();

private:
    UserMgr();
    void initHandlers();

    int m_status;
    std::shared_ptr<UserInfo> m_userInfo;
    QVector<std::shared_ptr<FriendApplyInfo>> m_friendApply_list;
    QVector<std::shared_ptr<FriendInfo>> m_firendInfo_list;
    QMap<int, std::shared_ptr<FriendInfo>> m_friendInfo_map;
    QString m_token;

};

#endif // USERMGR_H
