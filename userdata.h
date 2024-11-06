#ifndef USERDATA_H
#define USERDATA_H

#include "global.h"

class FriendInfo;
// 用户基本信息
struct UserInfo {
    UserInfo(int uid, QString name, QString icon);
    UserInfo(int uid, QString name, QString nick, QString icon, int sex);
    UserInfo(std::shared_ptr<FriendInfo> friendInfo);
    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    QPixmap _iconPixmap;
    int _sex;
    std::shared_ptr<FriendInfo> _friendInfo;

};

// 添加好友申请
class AddFriendApply {
public:
    AddFriendApply(int from_uid, QString name, QString desc,
                   QString icon, QString nick, int sex);
    int _from_uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    int     _sex;
};

// 申请好友信息
struct FriendApplyInfo {
    FriendApplyInfo(int uid, QString name, QString desc,QString icon, QString nick, int sex, int status);
    FriendApplyInfo(std::shared_ptr<AddFriendApply> addinfo);
    FriendApplyInfo(int uid, QString name, QString icon, QString backmsg, int sex);

    int _uid;
    QString _name;
    QString _desc;
    QString _icon;
    QString _nick;
    QString _back_msg;
    int _sex;
    int _status;
    QString _tag;
};
// 同意好友信息
struct FriendAuthInfo {
    FriendAuthInfo(int uid, QString name,
                   QString nick, QString icon, int sex);
    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
};
// 同意好友的回包信息
struct FriendAuthRsp {
    FriendAuthRsp(int peer_uid, QString peer_name,
            QString peer_nick, QString peer_icon, int peer_sex);


    int _uid;
    QString _name;
    QString _nick;
    QString _icon;
    int _sex;
};

// 好友信息
class FriendInfo : public QObject {
    Q_OBJECT
public:
    FriendInfo(QObject* parent = nullptr);
    ~FriendInfo();

    FriendInfo(int uid, QString name, QString nick, QString icon,
               int sex, QString desc, QString back, QString new_msg="");
    FriendInfo(std::shared_ptr<FriendAuthInfo> auth_info);
    FriendInfo(std::shared_ptr<FriendAuthRsp> auth_rsp);

    FriendInfo(const FriendInfo& info);

    void AppendChatMsg(std::shared_ptr<ChatMsgData> msg);
    void prependChatMsg(std::shared_ptr<ChatMsgData> msg);


    void UpdateChatMsgStatus(const QString& msgId, int status);

    QString newMsg() const;
    void setNewMsg(const QString &newNewMsg);

signals:
    void newMsgChanged(const QString &newNewMsg);

    void newTimeChanged(const QDateTime &newNewTime);

    void unreadMsgChanged(int newUnreadMsg);

    void allChatMsgCountChanged();

public:
    int m_uid;
    int m_sex;

    QString m_name;
    QString m_nick;
    QString m_icon;
    QString m_tag;
    QString m_desc;
    QString m_back;

    QPixmap m_iconPixmap;
    QVector<std::shared_ptr<ChatMsgData>> m_chat_msg;

    QDateTime newTime() const;
    void setNewTime(const QDateTime &newNewTime);

    int unreadMsg() const;
    void setUnreadMsg(int newUnreadMsg);

    int allChatMsgCount() const;
    void setAllChatMsgCount(int newAllChatMsgCount);

private:
    QString m_newMsg;
    QDateTime m_newTime;
    int m_unreadMsg;
    int m_allChatMsgCount = 9999;


};


#endif // USERDATA_H
