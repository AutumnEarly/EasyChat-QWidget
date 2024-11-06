#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QLinkedList>
#include "global.h"
#include "chatMsgList/chatmsgitembase.h"
#include "msgBubble/TextBubble.h"
#include "msgBubble/pictureBubble.h"
#include "tcpmgr.h"
#include "chatList/chatlist.h"
#include "chatList/chatlistitem.h"
#include "chatMsgList/chattingrecordslist.h"
#include <QQueue>
#include <QPair>

namespace Ui {
class ChatPage;
}

class ChatPage : public QWidget
{
    Q_OBJECT

public:
    explicit ChatPage(QWidget *parent = nullptr);
    ~ChatPage();
    void setUserInfo(std::shared_ptr<UserInfo> userInfo);
    std::shared_ptr<UserInfo> getUserInfo();

    ChattingRecordsList* getChatRecordsList();

    void appendChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd = true);
    void prependChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd = true);

signals:
    void sig_sticky_contact(std::shared_ptr<UserInfo> userInfo);

private slots:
    void on_send_msg_btn_clicked();

    void on_send_image_lb_clicked(QMouseEvent *);

private:
    template<typename T>
    using shared_ptr = std::shared_ptr<T>;


    Ui::ChatPage *ui;
    std::shared_ptr<UserInfo> m_userInfo;

    QTimer sendTestTimer;
};

#endif // CHATPAGE_H
