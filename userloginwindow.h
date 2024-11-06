#ifndef USERLOGINWINDOW_H
#define USERLOGINWINDOW_H

#include <QWidget>
#include <QPainterPath>
#include "global.h"

namespace Ui {
class UserLoginWindow;
}

class UserLoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserLoginWindow(QWidget *parent = nullptr);
    ~UserLoginWindow();
    // 初始化HTTP请求映射表
    void initHttpHandlers();
signals:
    void sig_connect_server(ServerInfo serverInfo);
private slots:
    void on_login_btn_clicked();
    void on_reg_btn_clicked();
    // 登录请求完成
    void slot_login_mod_finish(ReqId reqId, QString res, ErrorCodes err);
    // 连接服务器完成
    void slot_server_connect_finish(bool success);
    // 切换聊天窗口
    void slot_switch_ChatW();
protected:

private:
    Ui::UserLoginWindow *ui;
    // HTTP请求映射表
    QMap<ReqId, std::function<void(const QJsonObject&)>> m_handlers;

    // 暂存用户信息
    int m_uid;
    QString m_token;
};

#endif // USERLOGINWINDOW_H
