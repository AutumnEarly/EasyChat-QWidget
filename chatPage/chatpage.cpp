#include "chatpage.h"
#include "ui_chatpage.h"

#include "chatmainwindow.h"

ChatPage::ChatPage(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ChatPage)
{
    ui->setupUi(this);

}

ChatPage::~ChatPage()
{
    qDebug() << "delete chat Page";
    delete ui;
}

void ChatPage::setUserInfo(std::shared_ptr<UserInfo> userInfo)
{
    m_userInfo = userInfo;

    // TODO: 设置标题名
    ui->contact_name_lb->setText(m_userInfo->_name);
    this->setWindowTitle(UserMgr::GetInstance()->getName() + " - " + m_userInfo->_name);

    // 设置聊天框的所属用户信息
    ui->chat_records_list->setUserInfo(m_userInfo);


}

std::shared_ptr<UserInfo> ChatPage::getUserInfo()
{
    return m_userInfo;
}

ChattingRecordsList* ChatPage::getChatRecordsList()
{
    return ui->chat_records_list;
}

void ChatPage::appendChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd)
{
    ui->chat_records_list->appendChatMsg(userInfo,msgData, isAdd);

}

void ChatPage::prependChatMsg(std::shared_ptr<UserInfo> userInfo, std::shared_ptr<ChatMsgData> msgData, bool isAdd)
{
    ui->chat_records_list->prependChatMsg(userInfo,msgData, isAdd);
}


void ChatPage::on_send_msg_btn_clicked()
{
    if(m_userInfo == nullptr) {
        qDebug() << "send error! userInfo NULL!";
        return;
    }
    qDebug() << "self send msg";
    std::shared_ptr<UserInfo> selfInfo = UserMgr::GetInstance()->getUserInfo();
    QVector<MsgInfo> msgInfo = ui->send_msg_textEdit->getMsgList();
    QJsonObject textObj; // 用于存储文本消息的 JSON 对象
    QJsonArray textArray; // 用于存储文本消息数组
    int txt_size = 0; // 用于记录当前累计消息长度
    for(int i = 0; i < msgInfo.count();++i) {
        auto& msg = msgInfo[i];
        qDebug() << "flag " << msg.msgFlag << " content " << msg.content;
        // 检查消息内容长度是否合规，超过1024字符则跳过
        if(msg.content.length() > 1024){
            continue;
        }
        if(msg.content.trimmed().isEmpty()) {
            continue;
        }

        QString type = msg.msgFlag;

        // 生成唯一的消息ID
        QUuid uuid = QUuid::createUuid();
        QString uuidString = uuid.toString(QUuid::WithoutBraces); // 转为字符串
        // 将消息内容添加到文本消息数组中
        txt_size += msg.content.length();
        QJsonObject obj;
        QByteArray utf8Message = msg.content.toUtf8();
        obj["content"] = QString::fromUtf8(utf8Message);
        obj["msgid"] = uuidString;
        obj["type"] = msg.msgFlag;
        obj["status"] = 0x01;
        obj["isRead"] = 1;
        textArray.append(obj);

        // 等待消息内容超过最大长度，然后发送信息 或者已经到最后一条消息
        if(txt_size + msg.content.length() > 1024 || i == msgInfo.count()-1) {
            // 构建文本消息的 JSON 数据并发送给服务器
            textObj["fromuid"] = UserMgr::GetInstance()->getUid();
            textObj["touid"] = m_userInfo->_uid;
            textObj["text_array"] = textArray;
            textObj["type"] = msg.msgFlag;
            textObj["device"] = this_device_name;
            // 输出文本消息数组内容
            qDebug() << "textArray is " << textArray;

            QJsonDocument doc(textObj);
            QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
            txt_size = 0;
            textArray = QJsonArray();
            textObj = QJsonObject();
            // 发送 TCP 请求给 chat server
            if(type == "text") {

                emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_TEXT_CHAT_MSG_REQ, jsonData);
            } else if(type == "image") {

                emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_FILE_CHAT_MSG_REQ, jsonData);
            }
        }

        // TODO: 不管发送是否成功先将消息组件显示出去，tcpMgr会处理服务器的回包
        std::shared_ptr<ChatMsgData> msgData = std::make_shared<ChatMsgData>(uuidString,
            msg.content,UserMgr::GetInstance()->getUid(),m_userInfo->_uid,msg.msgFlag,QDateTime::currentDateTime());
        msgData->_status = 0x01;
        msgData->_is_read = true;

        this->appendChatMsg(selfInfo,msgData);

        // TODO: 将消息保存到本地
        // emit sig_save_chat_msg(user, msgData);

        // 更新最新消息
        UserMgr::GetInstance()->appendFriendChatMsg(msgData->_to_uid, msgData);
    }
    emit sig_sticky_contact(m_userInfo);
}



void ChatPage::on_send_image_lb_clicked(QMouseEvent *)
{
    sendTestTimer.setInterval(1);
    sendTestTimer.start();
    connect(&sendTestTimer,&QTimer::timeout,[this]() {
        ui->send_msg_textEdit->insertPlainText("QLinkedList 是 Qt 中的一个双向链表容器类，用于存储 C++ 类型的元素。它提供了对链表数据结构的高效操作，如插入和删除。这是一个模板类，因此可以用于存储任意类型的数据。QLinkedList 的常用接口有");
        this->on_send_msg_btn_clicked();
        sendTestTimer.start();
    });
}

