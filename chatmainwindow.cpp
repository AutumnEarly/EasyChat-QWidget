#include "chatmainwindow.h"
#include "ui_chatmainwindow.h"

#include "components/addfrienddialog.h"
#include "qctool.h"

ChatMainWindow::ChatMainWindow(QWidget *parent) :
    FramelessWidget(parent),
    ui(new Ui::ChatMainWindow)
{
    ui->setupUi(this);
    this->setContentsMargins(0,0,0,0);

    init();
    initStyleQss();
//    this->setResizeEnable(false);
    this->setTitleBar(ui->right_content_head);
    ui->side_chat_lb->setState("active");
    m_contactList = std::make_shared<ChatList>();
    m_contactList->setObjectName("contact_list");
    ui->left_content_stacked_w->addWidget(m_contactList.get());
    ui->left_content_stacked_w->setCurrentWidget(m_contactList.get());



    // 获取联系人列表子项点击事件
    connect(m_contactList.get(),&QListWidget::itemClicked,this,&ChatMainWindow::slot_chatListItem_clicked);

    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_notify_chat_msg,this,&ChatMainWindow::slot_notify_chat_msg);

    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_search_rsp,this,&ChatMainWindow::slot_search_rsp);

    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_account_other_login,this,&ChatMainWindow::slot_account_other_login);

    ui->search_input->clearFocus();
}

ChatMainWindow::~ChatMainWindow()
{
    qDebug() << "ChatMainWindow delete";
    delete ui;
}

void ChatMainWindow::init()
{
    // 测试信息
    QList<FriendInfo> list {
//        {1,"青雀","nick",":/img/qingque1.jpg",1,"","hi 青雀"},
//        {2,"符玄","nick",":/img/teresa.png",1,"","hi 符玄"},
//        {3,"Farushan","nick",":/img/farushan1.jpg",1,"","hi "},
//        {4,"青雀","nick",":/img/qingque1.jpg",1,"","hi 青雀"},
//        {5,"符玄","nick",":/img/teresa.png",1,"","hi 符玄"},
//        {6,"Farushan","nick",":/img/farushan1.jpg",1,"","hi "},
//        {7,"青雀","nick",":/img/qingque1.jpg",1,"","hi 青雀"},
//        {8,"符玄","nick",":/img/teresa.png",1,"","hi 符玄"},
//        {9,"Farushan","nick",":/img/farushan1.jpg",1,"","hi "},
//        {10,"Farushan","nick",":/img/farushan1.jpg",1,"","hi "},
//        {11,"青雀","nick",":/img/qingque1.jpg",1,"","hi 青雀"},
//        {31,"符玄","nick",":/img/teresa.png",1,"","hi 符玄"},
//        {141,"Farushan","nick",":/img/farushan1.jpg",1,"","hi "}
    };
    QJsonArray arr;
    for(const auto & i : list) {
        QJsonObject o;
        o["name"] = i.m_name;
        o["back"] = i.m_back;
        o["desc"] = i.m_desc;
        o["icon"] = i.m_icon;
        o["sex"] = i.m_sex;
        o["uid"] = i.m_uid;
        arr.append(o);
    }


    auto selfInfo = UserMgr::GetInstance()->getUserInfo();
    QPixmap pixmap(selfInfo->_icon);

    ui->side_head_portrait_lb->setPixmap(QCTool::RoundImage(pixmap.scaled(40,40,Qt::IgnoreAspectRatio,Qt::SmoothTransformation),40,40,8));
    ui->side_head_portrait_lb->setScaledContents(false);

    UserMgr::GetInstance()->appendFriendList(arr);
    QVector<std::shared_ptr<ChatMsgData>> msgData;
    for(int i = 0;i < 5;i++) {
        msgData.push_back(std::make_shared<ChatMsgData>("1020",":/img/self_head_icon.png",i,13,"image",QDateTime::currentDateTime()));
    }

}

void ChatMainWindow::initStyleQss()
{

    auto setStyle = [&](QWidget* w, const QString& url) {
        QFile file(url);
        if(file.open(QIODevice::ReadOnly)) {
            w->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "文件打开失败!";
        }
    };

    setStyle(ui->side_bar, ":/style/chat_main_w/left_content_side.qss");
    setStyle(ui->left_content_w, ":/style/chat_main_w/left_content.qss");
    setStyle(ui->right_content_w, ":/style/chat_main_w/right_content.qss");

}

void ChatMainWindow::slot_sticky_contact(std::shared_ptr<UserInfo> userInfo)
{
    qDebug() << "contact name " << userInfo->_name << " sticky!";

    m_contactList->swapItemPos(m_contactList->findItemIndex(userInfo),0);
}

void ChatMainWindow::mousePressEvent(QMouseEvent *event)
{
    setFocus();
}

void ChatMainWindow::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    m_userInfo = userInfo;
    qDebug() << m_userInfo->_name;

    this->setWindowTitle(UserMgr::GetInstance()->getName() + " - " + m_userInfo->_name);
}


void ChatMainWindow::slot_chatListItem_clicked(QListWidgetItem *item)
{

    QWidget* w = m_contactList->itemWidget(item);
    if(!w){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }
    ChatListItem* chatListItem = qobject_cast<ChatListItem*>(w);
    if(!chatListItem){
        qDebug()<< "slot item clicked widget is nullptr";
        return;
    }

    std::shared_ptr<UserInfo> userInfo = chatListItem->getUserInfo();
    this->setUserInfo(userInfo);
    qDebug()<< "contact item clicked " << userInfo->_uid;


    std::shared_ptr<UserInfo> selfInfo = UserMgr::GetInstance()->getUserInfo();
    QVector<std::shared_ptr<ChatMsgData>> msgInfos = UserMgr::GetInstance()->getFriendChatMsg(userInfo->_uid);

    auto chatPage = m_chatPages.find(m_userInfo);
    if(chatPage != m_chatPages.end()) {

        m_chatPage = m_chatPages[m_userInfo];
        ui->right_content_stacked_w->setCurrentWidget(m_chatPage.get());

        qDebug() << "switch chatPage " << m_chatPage.get();
    } else {
        m_chatPages[m_userInfo] = std::make_shared<ChatPage>();
        m_chatPage = m_chatPages[m_userInfo];
        m_chatPage->setUserInfo(userInfo);

        ui->right_content_stacked_w->addWidget(m_chatPage.get());
        ui->right_content_stacked_w->setCurrentWidget(m_chatPage.get());

        connect(m_chatPage.get(),&ChatPage::sig_sticky_contact, this,&ChatMainWindow::slot_sticky_contact);
        qDebug() << "create chatPage " << m_chatPage.get();
    }

}

void ChatMainWindow::slot_notify_chat_msg(std::shared_ptr<ChatMsg> msg, ChatMsgRole role)
{
    int count = m_contactList->count();
    qDebug() << "notify msg parse ... this contact count... " << count;

    for(int i = 0; i < count;i++) {
        // 1.查询推送的消息，在聊天栏是否存在
        QWidget* item = m_contactList->itemWidget(m_contactList->item(i));
        if(!item) {
            qDebug() << "chatlistitem error !"<< item;
            continue;
        }
        ChatListItem* chatItem = qobject_cast<ChatListItem*>(item);
        if(!chatItem) {
            qDebug() << "chatlistitem error !" << chatItem;
            continue;
        }

        std::shared_ptr<UserInfo> userInfo = chatItem->getUserInfo();
        // 2. 如果当前聊天项是推送消息项
        if(userInfo->_uid == msg->_from_uid) {

            std::shared_ptr<FriendInfo> friendInfo = UserMgr::GetInstance()->getFriendInfoById(msg->_from_uid);
            if(!friendInfo) {
                qDebug() << "friendInfo is null " << friendInfo.get();
            }
            for(int i = 0 ; i < msg->_msg_datas.count(); i++) {

                // 3. 为相应的好友添加历史消息
                UserMgr::GetInstance()->appendFriendChatMsg(userInfo->_uid, msg->_msg_datas[i]);
                qDebug() << "uid: " << userInfo->_uid << " friendInfo " << friendInfo.get();
                qDebug() << "add msg: " << friendInfo->m_chat_msg[friendInfo->m_chat_msg.count()-1]->_msg_content
                         << "type: " << friendInfo->m_chat_msg[friendInfo->m_chat_msg.count()-1]->_type;

                // 4. 创建消息组件
                if(m_userInfo && m_userInfo->_uid == msg->_from_uid) {
                    qDebug() << "add ChatMsgItem ";
                    m_chatPage->appendChatMsg(userInfo, msg->_msg_datas[i]);

                } else if(m_chatPages.contains(userInfo)) {
                    qDebug() << "add ChatMsgCache ";
                    m_chatPages[userInfo]->appendChatMsg(userInfo, msg->_msg_datas[i], false);
                }
            }

            // 将当前项设为置顶
            m_contactList->swapItemPos(i,0);
            return;
        }

    }

    std::shared_ptr<FriendInfo> friendInfo = UserMgr::GetInstance()->getFriendInfoById(msg->_from_uid);
    qDebug() << "not find contact... add contact " << friendInfo.get();

    for(int i = 0 ; i < msg->_msg_datas.count(); i++) {

        // 3. 为相应的好友添加历史消息
        UserMgr::GetInstance()->appendFriendChatMsg(friendInfo->m_uid, msg->_msg_datas[i]);

        m_chatPage->appendChatMsg(std::make_shared<UserInfo>(friendInfo), msg->_msg_datas[i],false);
    }

    // 将当前项设为置顶
    m_contactList->swapItemPos(count,0);
    return;

}

void ChatMainWindow::slot_search_rsp(QJsonObject obj)
{
    AddFriendDialog* addFriendDia = new AddFriendDialog(this);

    int uid = obj["uid"].toInt();
    int sex = obj["sex"].toInt();
    QString icon = obj["icon"].toString();
    QString nick = obj["nick"].toString();
    QString name = obj["name"].toString();

    std::shared_ptr<UserInfo> userInfo = std::make_shared<UserInfo>(uid, name, nick, icon, sex);

    qDebug() << "parse search rsp";

    addFriendDia->setUserInfo(userInfo);

    int x = (this->width() - addFriendDia->width()) / 2;
    int y = (this->height() - addFriendDia->height()) / 2;

    addFriendDia->move(x,y);
    addFriendDia->show();
}

void ChatMainWindow::slot_account_other_login()
{
    QApplication::quit();
}


void ChatMainWindow::on_search_input_returnPressed()
{

    QJsonObject obj;

    obj["uid"] = ui->search_input->text();
    QJsonDocument doc(obj);
    QByteArray data = doc.toJson();

    ui->search_input->clearFocus();
    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_SEARCH_USER_REQ, data);

}



void ChatMainWindow::on_search_lb_clicked(QMouseEvent *)
{
    qDebug() << "search text ";
    this->initStyleQss();
}


void ChatMainWindow::on_side_chat_lb_clicked(QMouseEvent *)
{

    ui->side_contact_lb->setState("normal");
    ui->side_chat_lb->setState("active");

    ui->left_content_stacked_w->setCurrentWidget(m_contactList.get());

    qDebug() << "切换到聊天页";
    if(m_chatPage) {
        ui->right_content_stacked_w->setCurrentWidget(m_chatPage.get());
        ui->right_content_stacked_w->currentWidget()->show();

    } else if(m_friendApplyList){
        m_friendApplyList->hide();
    }
    qDebug() << "contact list isHide " << m_friendApplyList->isHidden();

}


void ChatMainWindow::on_side_contact_lb_clicked(QMouseEvent *)
{
    qDebug() << "切换到好友页";
    ui->side_chat_lb->setState("normal");
    ui->side_contact_lb->setState("active");
    if(!m_contactFriendList) {
        m_contactFriendList = std::make_shared<ContactFriendList>();
        m_contactFriendList->setObjectName("contact_friend_list");
        ui->left_content_stacked_w->addWidget(m_contactFriendList.get());

        m_friendApplyList = std::make_shared<FriendApplyList>();
        m_friendApplyList->setObjectName("friend_apply_list");
        ui->right_content_stacked_w->addWidget(m_friendApplyList.get());
    }
    m_friendApplyList->loadCache();
    ui->left_content_stacked_w->setCurrentWidget(m_contactFriendList.get());
    ui->right_content_stacked_w->setCurrentWidget(m_friendApplyList.get());
    m_friendApplyList->show();
}


void ChatMainWindow::on_quit_btn_clicked(QMouseEvent *)
{
    this->close();
}


void ChatMainWindow::on_max_show_win_lb_clicked(QMouseEvent *)
{
    if(this->isMaximized()) {
        this->showNormal();
    } else {
        this->showMaximized();
    }
}


void ChatMainWindow::on_min_show_win_lb_clicked(QMouseEvent *)
{
    this->showMinimized();
}

