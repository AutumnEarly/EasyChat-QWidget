#ifndef CHATMAINWINDOW_H
#define CHATMAINWINDOW_H

#include <QWidget>
#include <QListWidget>
#include "global.h"
#include "chatMsgList/chatmsgitembase.h"
#include "msgBubble/TextBubble.h"
#include "msgBubble/pictureBubble.h"
#include "tcpmgr.h"
#include "chatList/chatlist.h"
#include "chatList/chatlistitem.h"
#include "chatMsgList/chattingrecordslist.h"
#include "chatPage/chatpage.h"
#include "friendList/contactfriendlist.h"
#include "friendList/friendapplylist.h"
#include <QElapsedTimer>
#include <QTimer>

#include "./framelesscore/framelesswidget.h"

namespace Ui {
class ChatMainWindow;
}

class ChatMainWindow : public FramelessWidget
{
    Q_OBJECT

public:
    explicit ChatMainWindow(QWidget *parent = nullptr);
    ~ChatMainWindow();

    void init();

    void setUserInfo(std::shared_ptr<UserInfo> userInfo);

    void initStyleQss();

    void slot_sticky_contact(std::shared_ptr<UserInfo> userInfo);
protected:
    void mousePressEvent(QMouseEvent *event) override;

private slots:
    void slot_chatListItem_clicked(QListWidgetItem *item);

    void slot_notify_chat_msg(std::shared_ptr<ChatMsg> msg, ChatMsgRole role);

    void slot_search_rsp(QJsonObject obj);

    void slot_account_other_login();

    void on_search_input_returnPressed();

    void on_search_lb_clicked(QMouseEvent *);

    void on_side_chat_lb_clicked(QMouseEvent *);

    void on_side_contact_lb_clicked(QMouseEvent *);

    void on_quit_btn_clicked(QMouseEvent *);

    void on_max_show_win_lb_clicked(QMouseEvent *);

    void on_min_show_win_lb_clicked(QMouseEvent *);

signals:
    void sig_switch_chatMsg_list(ChattingRecordsList* item);
private:
    Ui::ChatMainWindow *ui;

    std::shared_ptr<UserInfo> m_userInfo;

    QMap<std::shared_ptr<UserInfo>, std::shared_ptr<ChatPage>> m_chatPages;

    std::shared_ptr<ChatPage> m_chatPage;
    std::shared_ptr<ChatList> m_contactList;
    std::shared_ptr<ContactFriendList> m_contactFriendList;
    std::shared_ptr<FriendApplyList> m_friendApplyList;
};

#endif // CHATMAINWINDOW_H
