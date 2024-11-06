#ifndef CONTACTUSERITEM_H
#define CONTACTUSERITEM_H

#include <QWidget>
#include <QBitmap>
#include "userdata.h"
#include "global.h"
#include "../components/roundimage.h"

namespace Ui {
class ChatListItem;
}


class ChatListItem : public QWidget
{
    Q_OBJECT
public:
    explicit ChatListItem(QWidget *parent = nullptr);
    ~ChatListItem();
    void setInfo(const QString& name,const QString& new_msg,const QDateTime& new_time, const int unread_msg_cnt, const QPixmap& pix);
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);
    void swapInfo(ChatListItem* item);

    std::shared_ptr<UserInfo> getUserInfo();
    QString name() const;
    void setName(const QString &newName);

    QString newMsg() const;
    void setNewMsg(const QString &newNewMsg);

    QDateTime newTime() const;
    void setNewTime(const QDateTime &newNewTime);

    int unreadMsgCnt() const;
    void setUnreadMsgCnt(int newUnreadMsgCnt);


    QPixmap icon() const;
    void setIcon(const QPixmap &newIcon);

    std::shared_ptr<FriendInfo> friendInfo() const;
    void setFriendInfo(const std::shared_ptr<FriendInfo> &newFriendInfo);

signals:
    void sig_switch_contactInfo(int id);
    void nameChanged();

    void newMsgChanged();

    void newTimeChanged(ChatListItem* item, const QDateTime &newNewTime);

    void unreadMsgCntChanged();

    void iconChanged();

    void friendInfoChanged();

private:
    Ui::ChatListItem *ui;
    std::shared_ptr<UserInfo> m_userInfo;
    std::shared_ptr<FriendInfo> m_friendInfo;
    QString m_name;
    QString m_newMsg;
    QDateTime m_newTime;
    int m_unreadMsgCnt;
    QPixmap m_icon;



};

#endif // CONTACTUSERITEM_H
