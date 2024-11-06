#include "tcpmgr.h"


TcpMgr::TcpMgr() : m_host(""), m_port(0), m_b_recv_pending(false),
    m_message_id(0), m_message_len(0)
{

    m_heartBeat_timer.setInterval(5000);

    // 绑定连接成功信号
    connect(&m_socket,&QTcpSocket::connected,this,[&]() {
        qDebug() << "tcp socket connected server !";

        emit sig_con_success(true);
        emit sig_send_heartBeat_msg();
    },Qt::UniqueConnection);

    // 绑定连接断开信号
    connect(&m_socket,&QTcpSocket::disconnected,[&]() {
        qDebug() << "tcp socket disconnected server !";
        emit sig_disconnect();
    });
    // 捕获错误
    connect(&m_socket, QOverload<QAbstractSocket::SocketError>::of(&QTcpSocket::errorOccurred), [&](QAbstractSocket::SocketError socketError) {
        Q_UNUSED(socketError)
        qDebug() << "Error:" << m_socket.errorString();

    });
    // 服务器数据处理
    connect(&m_socket,&QTcpSocket::readyRead,[&]() {
        // 有新的数据时，把全部数据存入缓冲区里
        m_buffer.append(m_socket.readAll());

        QDataStream stream(&m_buffer, QIODevice::ReadOnly);
        stream.setVersion(QDataStream::Qt_5_15);
        forever {
            // 先解析消息头部
            if(!m_b_recv_pending) {
                // 检查缓冲区中的数据是否足够解析出一个消息头（消息ID + 消息长度）, 不够退出函数，继续等待
                if(m_buffer.size() < static_cast<int>(sizeof(qint16) *2)) {
                    return;
                }
                // 读取消息ID和数据长度
                stream >> m_message_id >> m_message_len;
                // 把缓冲区的消息头部数据移除
                m_buffer = m_buffer.mid(sizeof(qint16)*2);
                // 输出读取的数据
                qDebug() << "Message ID:" << m_message_id << ", Length:" << m_message_len;
            }
            // 检查缓冲区剩余长度是否足够解析出完整消息体，不足则等待接收更多数据
            if(m_buffer.size() < m_message_len) {
                m_b_recv_pending = true;
                return;
            }
            m_b_recv_pending = false;
            // 读取消息体
            QByteArray msgBody = m_buffer.mid(0,m_message_len);

//            qDebug() << "Received body message is len " << msgBody.length() <<
//                "data is " << msgBody;

            // 从缓冲区中移除已处理的消息体数据
            m_buffer = m_buffer.mid(m_message_len);
            // 调用处理消息的函数
            handleMsg(ReqId(m_message_id), m_message_len, msgBody);
        }
    });
    // 发送数据绑定
    connect(this, &TcpMgr::sig_send_data, this, &TcpMgr::slot_send_data);

    connect(this,&TcpMgr::sig_restart_connect,this,&TcpMgr::slot_restart_connect,Qt::UniqueConnection);

    connect(&m_socket,&QTcpSocket::stateChanged,this,[&](const QAbstractSocket::SocketState& sockectState){

    });
    connect(this,&TcpMgr::sig_send_heartBeat_msg,this,&TcpMgr::slot_send_heartBeat_msg,Qt::UniqueConnection);

    connect(&m_heartBeat_timer,&QTimer::timeout,this,[&]() {
        if(m_b_heartBeat_pending) {
            UserMgr::GetInstance()->setStatus(-1);
            if(!m_b_reConnect) {
                // todo 重连准备
                emit sig_restart_connect();

            } else {
                m_b_reConnect = true;
            }
        } else {
            UserMgr::GetInstance()->setStatus(0);
            m_heartBeat_timer.setInterval(8000);
            m_heartBeat_timer.start();
        }
        m_b_heartBeat_pending = true;
        qDebug() << "pending heartBeart ...";
    });
    // 注册映射处理
    initHandlers();


}

void TcpMgr::initHandlers()
{
    // 处理登录回包
    m_handlers.insert(ReqId::ID_CHAT_LOGIN_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "Failed to create QJsonDocument.";
            emit sig_logon_err(ErrorCodes::ERR_JSON);
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            emit sig_logon_err(ErrorCodes::ERR_JSON);
            return;
        }
        auto uid = obj["uid"].toInt();
        auto name = obj["name"].toString();
        auto token = obj["token"].toString();
        auto nick = obj["nick"].toString();
        auto icon = obj["icon"].toString();
        auto sex = obj["sex"].toInt();
        auto userInfo = std::make_shared<UserInfo>(uid, name, nick, icon, sex);

        UserMgr::GetInstance()->setUserInfo(userInfo);
        UserMgr::GetInstance()->setToken(token);
        // 添加申请好友列表
        if(obj.contains("apply_list")) {
            UserMgr::GetInstance()->appendApplyList(obj["apply_list"].toArray());
        }
        //添加好友列表
        if (obj.contains("friend_list")) {
            UserMgr::GetInstance()->appendFriendList(obj["friend_list"].toArray());
        }
        qDebug() << "chat login RSP data is " << obj;
        emit sig_switch_ChatW();
    });

    // 发送文本消息请求的回包
    m_handlers.insert(ReqId::ID_TEXT_CHAT_MSG_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << " ";
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        auto from_uid = obj["fromuid"].toInt();
        auto touid = obj["touid"].toInt();
        auto friendInfo = UserMgr::GetInstance()->getFriendInfoById(touid);
        auto msgs = obj["text_array"].toArray();

        for(auto it : msgs) {
            QJsonObject obj = it.toObject();
            auto content = obj["content"].toString();
            QString msgid = obj["msgid"].toString();
            QString msgType = obj["type"].toString();

            // TODO: 更新消息状态 (还没想好)


            qDebug() << "friend " << friendInfo->m_name << " add msg " <<
                friendInfo->m_chat_msg.at(friendInfo->m_chat_msg.count()-1)->_msg_content;
        }

        qDebug() << "text chat msg rsp data is " << obj;
    });
    m_handlers.insert(ReqId::ID_NOTIFY_TEXT_CHAT_MSG_REQ,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << "";
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "notify TEXT chat msg req data is " << obj;
        auto fromuid = obj["fromuid"].toInt();
        auto touid = obj["touid"].toInt();
        auto text_array = obj["text_array"].toArray();

        auto p_msg = std::make_shared<ChatMsg>(fromuid,touid,text_array);
        emit sig_notify_chat_msg(p_msg, ChatMsgRole::Other);
    });

    // 发送文件消息请求的回包
    m_handlers.insert(ReqId::ID_FILE_CHAT_MSG_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        auto from_uid = obj["fromuid"].toInt();
        auto touid = obj["touid"].toInt();
        auto friendInfo = UserMgr::GetInstance()->getFriendInfoById(touid);
        auto msgs = obj["text_array"].toArray();

        for(auto it : msgs) {
            QJsonObject obj = it.toObject();
            auto content = obj["content"].toString();
            auto msgid = obj["msgid"].toString();
            auto msgType = obj["type"].toString();
            QDateTime msgDate = QDateTime::fromString(obj["type"].toString(),"yyyy-MM-dd hh:mm:ss");
            auto msgPtr = std::make_shared<ChatMsgData>(msgid,content,from_uid,touid,msgType,msgDate);

            friendInfo->AppendChatMsg(msgPtr);
            qDebug() << "friend " << friendInfo->m_name << " add msg " <<
                friendInfo->m_chat_msg.at(friendInfo->m_chat_msg.count()-1)->_msg_content;
        }
        qDebug() << "FILE chat msg rsp data is " << obj;
    });
    m_handlers.insert(ReqId::ID_NOTIFY_FILE_CHAT_MSG_REQ,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        qDebug() << " ";
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "notify FILE chat msg req data is " << obj;

        auto fromuid = obj["fromuid"].toInt();
        auto touid = obj["touid"].toInt();
        auto text_array = obj["text_array"].toArray();

        auto p_msg = std::make_shared<ChatMsg>(fromuid,touid,text_array);
        emit sig_notify_chat_msg(p_msg, ChatMsgRole::Other);
    });

    // 搜索用户回包
    m_handlers.insert(ReqId::ID_SEARCH_USER_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "search user rsp data is " << obj;

        emit sig_search_rsp(obj);
    });

    // 处理添加朋友申请回包
    m_handlers.insert(ReqId::ID_ADD_FRIEND_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "addFriend rsp data is " << obj;

    });
    // 处理添加朋友通知
    m_handlers.insert(ReqId::ID_NOTIFY_ADD_FRIEND_REQ,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "ID_NOTIFY_ADD_FRIEND_REQ data is " << obj;

        int uid = obj["applyuid"].toInt();
        QString name = obj["name"].toString();
        QString icon = obj["icon"].toString();
        QString descr = obj["descr"].toString();
        QString tag = obj["tag"].toString();
        int sex = obj["sex"].toInt();

        std::shared_ptr<FriendApplyInfo> info = std::make_shared<FriendApplyInfo>(uid, name, icon, descr, sex);
        info->_tag = tag;
        UserMgr::GetInstance()->appendApplyList(info);
        emit sig_notify_addFriendApply(info);

    });

    // 好友申请同意回包
    m_handlers.insert(ReqId::ID_AUTH_FRIEND_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "ID_AUTH_FRIEND_RSP data is " << obj;
        UserMgr::GetInstance()->addFriendInfo(obj);
        emit UserMgr::GetInstance()->sig_addFriend(UserMgr::GetInstance()->getFriendInfoById(obj["uid"].toInt()));


    });

    // 好友申请同意通知
    m_handlers.insert(ReqId::ID_NOTIFY_AUTH_FRIEND_REQ,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);
        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "notify auth friend rsp data is " << obj;
        std::shared_ptr<FriendInfo> info = std::make_shared<FriendInfo>();
        int uid = obj["fromuid"].toInt();

        QJsonObject jsObj;
        jsObj["name"] = obj["name"];
        jsObj["icon"] = obj["icon"];
        jsObj["nickname"] = obj["nickname"];
        jsObj["tag"] = obj["tag"];
        jsObj["sex"] = obj["sex"];
        jsObj["uid"] = uid;

        UserMgr::GetInstance()->addFriendInfo(jsObj);
        emit UserMgr::GetInstance()->sig_addFriend(UserMgr::GetInstance()->getFriendInfoById(uid));

    });

    // 心跳回包
    m_handlers.insert(ReqId::ID_HEARTBEAT_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);

        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }

        QJsonObject reqObj;
        reqObj["device"] = this_device_name;
        reqObj["uid"] = UserMgr::GetInstance()->getUid();

        m_b_heartBeat_pending = false;
        emit sig_send_data(ReqId::ID_HEARTBEAT_REQ, QJsonDocument(reqObj).toJson());
        UserMgr::GetInstance()->setStatus(0);
    });

    // 账号在其他地方登录
    m_handlers.insert(ReqId::ID_NOTIFY_OTHER_LOGIN_REQ,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);

        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "账号在其他地方登录!";

        emit sig_account_other_login();
    });

    // 获取聊天数据
    m_handlers.insert(ReqId::ID_GET_CHAT_MSG_RSP,[&](ReqId id, int len, QByteArray data) {
        Q_UNUSED(len);

        QJsonDocument doc(QJsonDocument::fromJson(data));
        QJsonObject obj = doc.object();

        if(doc.isNull()) {
            qDebug() << "parse json error ";
            return;
        }
        auto err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "error is " << err;
            return;
        }
        qDebug() << "ID_GET_CHAT_MSG_RSP is " << obj;

        if(obj["msg_list"].isArray()) {

            QVector<std::shared_ptr<ChatMsgData>> newMsgs;
            QJsonArray msgList = obj["msg_list"].toArray();

            int friendId = obj["uid"].toInt();
            int msgCount = msgList.count() - 1;
            int isrecord = UserMgr::GetInstance()->getFriendChatMsg(friendId).count();
            if(msgList.isEmpty()) {
                auto friendInfo = UserMgr::GetInstance()->getFriendInfoById(friendId);
                friendInfo->setAllChatMsgCount(friendInfo->m_chat_msg.count());
            }

            for(int i = 0;i <= msgCount; i++) {
                auto msgObj = msgList.at(msgCount - i).toObject();

                std::shared_ptr<ChatMsgData> msgData = std::make_shared<ChatMsgData>();
                int from_uid = msgObj["from_uid"].toInt();
                int to_uid = msgObj["to_uid"].toInt();
                QDateTime dateTime = QDateTime::fromString(msgObj["datetime"].toString(),"yyyy-MM-dd hh:mm:ss");

                msgData->_from_uid = from_uid;
                msgData->_to_uid = to_uid;

                msgData->_status = msgObj["status"].toInt();
                msgData->_is_read = msgObj["isRead"].toInt();

                msgData->_msg_content = msgObj["content"].toString();
                msgData->_type = msgObj["type"].toString();
                msgData->_dateTime = dateTime;

                newMsgs.push_back(msgData);

                if(isrecord) {
                    UserMgr::GetInstance()->getFriendInfoById(friendId)->prependChatMsg(msgData);
                } else {
                    UserMgr::GetInstance()->getFriendInfoById(friendId)->AppendChatMsg(msgData);
                }
            }

            emit UserMgr::GetInstance()->sig_appendedFriendChatMsg(friendId,newMsgs);
        }
    });
}

void TcpMgr::handleMsg(ReqId id, int len, QByteArray data)
{
    auto findIter = m_handlers.find(id);
    if(findIter == m_handlers.end()) {
        qDebug()<< "not found id ["<< id << "] to handle";

        return;
    }
    findIter.value()(id,len,data);
}


TcpMgr::~TcpMgr()
{

}

void TcpMgr::slot_connect_server(ServerInfo serverInfo)
{
    // 向服务器发送连接请求
    m_host = serverInfo.host;
    m_port = static_cast<uint16_t>(serverInfo.port.toUInt());

    qDebug() << "connecting to server ... host: " << serverInfo.host << " port: " << serverInfo.port;
    m_socket.connectToHost(m_host, m_port);
}

void TcpMgr::slot_send_heartBeat_msg()
{
    m_heartBeat_timer.start();
}

void TcpMgr::slot_restart_connect()
{
    qDebug() << "restart connect ... host: " << m_host << " port: " << m_port;
//    m_socket.connectToHost(m_host, m_port);
}

void TcpMgr::slot_send_data(ReqId id, QByteArray data)
{
    uint16_t reqId = id;
    quint16 len = static_cast<quint16>(data.length());

    QByteArray sendData;
    QDataStream out(&sendData, QIODevice::WriteOnly);
    // 将数据设置为网络字节
    out.setByteOrder(QDataStream::BigEndian);
    // 写入ID和长度
    out << reqId << len;
    // 添加数据
    sendData.append(data);
    // 发送
    m_socket.write(sendData);

    qDebug() << "tcpMgr send byte data is " << sendData;

}
