#ifndef TCPMGR_H
#define TCPMGR_H

#include <QObject>
#include <QTcpSocket>
#include <QByteArray>
#include <QJsonDocument>
#include <QTimer>
#include <QApplication>
#include "global.h"
#include "singleton.h"
#include "usermgr.h"

class TcpMgr : public QObject, public Singleton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr>
{
    Q_OBJECT
    friend class Singleton<TcpMgr>;
public:
    ~TcpMgr();


public slots:
    void slot_connect_server(ServerInfo serverInfo);
private slots:
    void slot_send_heartBeat_msg();
    void slot_restart_connect();

signals:
    // 连接完成
    void sig_con_success(bool success);
    // 断开连接
    void sig_disconnect();
    // 发送数据
    void sig_send_data(ReqId id, QByteArray data);
    // 切换页面
    void sig_switch_ChatW();
    void sig_logon_err(ErrorCodes err);

    void sig_send_heartBeat_msg();
    void sig_restart_connect();

    // 账号已在其他地方登录
    void sig_account_other_login();

    // 消息通知
    void sig_notify_chat_msg(std::shared_ptr<ChatMsg> msg, ChatMsgRole role);
    // 搜索结果
    void sig_search_rsp(QJsonObject obj);
    // 申请添加好友请求通知
    void sig_notify_addFriendApply(std::shared_ptr<FriendApplyInfo> info);
private slots:
    void slot_send_data(ReqId id, QByteArray data);
private:
    TcpMgr();
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);

    QTcpSocket m_socket;
    QString m_host;
    uint16_t m_port;
    QByteArray m_buffer;
    bool m_b_recv_pending;
    quint16 m_message_id;
    quint16 m_message_len;
    QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> m_handlers;

    bool m_b_heartBeat_pending;
    QTimer m_heartBeat_timer;

    bool m_b_reConnect;
};

#endif // TCPMGR_H
