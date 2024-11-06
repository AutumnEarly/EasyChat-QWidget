#include "contactfriendlist.h"
#include "ui_contactfriendlist.h"

#include "usermgr.h"

ContactFriendList::ContactFriendList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContactFriendList)
{
    ui->setupUi(this);

    m_friendList = new QListWidget();
    m_friendList->horizontalScrollBar()->hide();
    m_friendList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    m_friendList->setContentsMargins(0,0,0,0);  
    m_friendList->setObjectName("friend_list");

    this->layout()->addWidget(m_friendList);

    auto friendInfos = UserMgr::GetInstance()->getFriendInfo();
    foreach(auto info, friendInfos) {
        m_data.push_back(info);

        FriendListItem* item = new FriendListItem();
        item->setInfo(info->m_icon,info->m_name);

        QListWidgetItem* listItem = new QListWidgetItem(m_friendList);
        listItem->setSizeHint(QSize(130,60));
        m_friendList->addItem(listItem);
        m_friendList->setItemWidget(listItem, item);

    }
    connect(UserMgr::GetInstance().get(),&UserMgr::sig_addFriend,this,[&](std::shared_ptr<FriendInfo> info) {
        m_data.push_back(info);
        m_cache.push_back(info);
        load();
    });
}

ContactFriendList::~ContactFriendList()
{
    delete ui;
}

void ContactFriendList::appendData(std::shared_ptr<FriendInfo> info, bool isAdd)
{
    m_data.push_back(info);
    if(isAdd) {
        FriendListItem* item = new FriendListItem();
        item->setInfo(info->m_icon,info->m_name);

        QListWidgetItem* listItem = new QListWidgetItem(m_friendList);
        listItem->setSizeHint(QSize(130,60));
        m_friendList->addItem(listItem);
        m_friendList->setItemWidget(listItem, item);
    } else {
       m_cache.push_back(info);
    }

}

void ContactFriendList::insertData(int index, std::shared_ptr<FriendInfo> info)
{

}

void ContactFriendList::removeData(int index)
{
    QListWidgetItem* item = m_friendList->takeItem(index);
    m_data.remove(index);
    if(item) {
        delete item;
    }
}

void ContactFriendList::load()
{
    foreach (auto info, m_cache) {
        FriendListItem* item = new FriendListItem();
        item->setInfo(info->m_icon,info->m_name);

        QListWidgetItem* listItem = new QListWidgetItem(m_friendList);
        listItem->setSizeHint(QSize(130,60));
        m_friendList->addItem(listItem);
        m_friendList->setItemWidget(listItem, item);
    }
    m_cache.clear();
}
