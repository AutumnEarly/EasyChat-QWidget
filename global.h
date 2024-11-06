#ifndef GLOBAL_H
#define GLOBAL_H

#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QString>
#include <QByteArray>
#include <QFile>
#include <QSettings>
#include <QDir>
#include <iostream>
#include <memory>
#include <QPixmap>
#include <QDateTime>
#include <QDateTime>

enum ReqId {
    ID_GET_VARIFY_CODE = 1001, //获取验证码
    ID_REG_USER = 1002, //注册用户
    ID_RESET_PWD = 1003, //重置密码
    ID_LOGIN_USER = 1004, //用户登录
    ID_CHAT_LOGIN = 1005, //登陆聊天服务器
    ID_CHAT_LOGIN_RSP = 1006, //登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007, //用户搜索请求
    ID_SEARCH_USER_RSP = 1008, //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,  //添加好友申请
    ID_ADD_FRIEND_RSP = 1010, //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,  //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,  //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,  //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息
    ID_FILE_CHAT_MSG_REQ = 1020, //文件聊天信息请求
    ID_FILE_CHAT_MSG_RSP = 1021, //文件聊天信息回复
    ID_NOTIFY_FILE_CHAT_MSG_REQ = 1022, //通知用户文件聊天信息

    ID_NOTIFY_OTHER_LOGIN_REQ = 1023, // 通知用户账号在异地登录

    ID_GET_CHAT_MSG_REQ = 1100, // 获取聊天记录
    ID_GET_CHAT_MSG_RSP = 1101,

    ID_HEARTBEAT_RSP = 2001, // 心跳
    ID_HEARTBEAT_REQ = 2002, // 心跳
};
enum Modules{
    REGISTERMOD = 0,
    RESETMOD = 1,
    LOGINMOD = 2,
};
enum ErrorCodes {
    SUCCESS = 0,
    ERR_JSON = 1, // json 解析失败
    ERR_NETWORK = 2 // 网络错误
};

// 消息角色
enum ChatMsgRole {
    Self = 0, // 自己
    Other = 1 // 其他
};
// 消息信息
struct MsgInfo {
    QString msgFlag; //"text,image,file"
    QString content; //表示文件和图像的url,文本信息
    QPixmap pixmap;  //文件和图片的缩略图
};

class ChatMsgData {
public:
    ChatMsgData() {}
    ChatMsgData(QString msg_id, QString msg_content, int fromuid, int touid, QString type, QDateTime datetime)
        :_msg_id(msg_id),_msg_content(msg_content) ,_type(type),
        _from_uid(fromuid), _to_uid(touid), _status(0), _dateTime(datetime), _is_read(true) {
    }
    QString _msg_id;
    QString _msg_content;
    QString _type;
    QDateTime _dateTime;
    int _from_uid;
    int _to_uid;
    int _status;
    bool _is_read;
};

class ChatMsg {
public:
    ChatMsg(int fromuid, int touid, QJsonArray msgs) : _from_uid(fromuid), _to_uid(touid) {
        for(auto it : msgs) {
            QJsonObject obj = it.toObject();
            auto content = obj["content"].toString();
            auto msgid = obj["msgid"].toString();
            auto msgType = obj.contains("type") ? obj["type"].toString(): "text";
            QDateTime datetime = QDateTime::fromString(obj["datetime"].toString(),"yyyy-MM-dd hh:mm:ss");

            auto msgPtr = std::make_shared<ChatMsgData>(
                msgid,content,_from_uid,_to_uid,msgType,datetime);

            _msg_datas.push_back(msgPtr);
        }
    }
    int _from_uid;
    int _to_uid;
    QVector<std::shared_ptr<ChatMsgData>> _msg_datas;
};

struct ServerInfo {
    QString host;
    QString port;
    QString token;
    int uid;
};
extern QString gate_url_prefix;
extern QString this_device_name;
#endif // GLOBAL_H
