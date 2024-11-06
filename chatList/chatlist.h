
#ifndef CHATLIST_H
#define CHATLIST_H

#include <QListView>
#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QPushButton>
#include <QStandardItemModel>
#include <QPainter>
#include <iostream>
#include <QScrollBar>
#include <QTimer>
#include <QQueue>
#include "userdata.h"


/*********************************************************************************
  * @Version:
  * @FileName:      ChatList.h
  * @Brief:         联系人列表视图

  * @Author:        秋初
  * @Date:          2024-09-09
  * @History:
**********************************************************************************/

class ChatListItem;
class ChatList : public QListWidget
{
    Q_OBJECT
public:
    explicit ChatList(QWidget * parent = nullptr);
    int swapItemPos(int index1, int index2);

    ChatListItem* findItem(std::shared_ptr<UserInfo> userInfo);
    int findItemIndex(std::shared_ptr<UserInfo> userInfo);

    void appendItem(std::shared_ptr<FriendInfo> info);
    void removeItem(std::shared_ptr<UserInfo> userInfo);
protected:
    bool eventFilter(QObject *watched, QEvent *event) override;
signals:

private slots:
    void slot_chatItemInfoChanged(ChatListItem* item);
private:
    int m_count;
    int m_loadCount;

    // 异步请求聊天记录
    QTimer m_timer;
    QQueue<std::shared_ptr<QJsonObject>> m_cacheData;
};

#endif // CHATLIST_H
