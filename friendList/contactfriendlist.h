#ifndef CONTACTFRIENDLIST_H
#define CONTACTFRIENDLIST_H

#include <QWidget>
#include <QScrollBar>
#include <QListWidget>
#include <QListWidgetItem>
#include <QQueue>
#include "../usermgr.h"
#include "./friendlistitem.h"

namespace Ui {
class ContactFriendList;
}

class ContactFriendList : public QWidget
{
    Q_OBJECT

public:
    explicit ContactFriendList(QWidget *parent = nullptr);
    ~ContactFriendList();

    void appendData(std::shared_ptr<FriendInfo> info, bool isAdd = true);
    void insertData(int index,std::shared_ptr<FriendInfo> info);
    void removeData(int uid);
    void load();
private slots:

private:
    Ui::ContactFriendList *ui;

    QListWidget* m_friendList;
    QVector<std::shared_ptr<FriendInfo>> m_data;
    QQueue<std::shared_ptr<FriendInfo>> m_cache;
};

#endif // CONTACTFRIENDLIST_H
