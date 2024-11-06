#include "userloginwindow.h"
#include "ui_userloginwindow.h"
#include "global.h"
#include "httpmgr.h"
#include "mainwindow.h"
#include "tcpmgr.h"

UserLoginWindow::UserLoginWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserLoginWindow)
{
    ui->setupUi(this);

    QPixmap pix(":/img/self_head_icon.png");
    ui->head_portrait->setPixmap(pix.scaled(150,150,Qt::IgnoreAspectRatio,Qt::SmoothTransformation));

    // 绑定连接服务器信号
    connect(this,&UserLoginWindow::sig_connect_server,TcpMgr::GetInstance().get(),&TcpMgr::slot_connect_server);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_con_success, this,&UserLoginWindow::slot_server_connect_finish);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_switch_ChatW,this,&UserLoginWindow::slot_switch_ChatW);
    initHttpHandlers();

    ui->user_input->setText("qingque@hk.com");
    ui->pwd_input->setText("123456");

}

UserLoginWindow::~UserLoginWindow()
{
    delete ui;
}

void UserLoginWindow::initHttpHandlers()
{
    // 添加登录映射
    m_handlers.insert(ReqId::ID_LOGIN_USER,[this](const QJsonObject& obj) {
        int err = obj["error"].toInt();
        if(err != ErrorCodes::SUCCESS) {
            qDebug() << "user login error: " << err;
            return;
        }
        ServerInfo serverInfo;
        serverInfo.host = obj["host"].toString();
        serverInfo.port = obj["port"].toString();
        m_uid = obj["uid"].toInt();
        m_token = obj["token"].toString();

        qDebug() << "email login "<< obj["email"].toString() << "uid is" << serverInfo.uid;
        qDebug() << "connect server info host is "<< serverInfo.host << " port is" << serverInfo.port
                 << "token is " << serverInfo.token;
        // 发送连接服务器信号，让TcpMgr绑定连接
        emit sig_connect_server(serverInfo);
    });
}


void UserLoginWindow::on_login_btn_clicked()
{
    // 切换为聊天主页面

    qDebug() << "登录操作";
    QJsonObject obj;
    obj["email"] = ui->user_input->text();
    obj["passwd"] = ui->pwd_input->text();
    obj["device"] = "pc";
    obj["location"] = "重庆";
    QUrl url(gate_url_prefix+"/user_login");

    // 连接登录完成请求信号
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_login_mod_finish,this,
            &UserLoginWindow::slot_login_mod_finish, Qt::UniqueConnection);

    HttpMgr::GetInstance()->PostHttpReq(url,obj,ReqId::ID_LOGIN_USER,Modules::LOGINMOD);

}

void UserLoginWindow::on_reg_btn_clicked()
{
    // 切换为注册页面

    MainWindow* parentW = qobject_cast<MainWindow*>(this->window());
    parentW->selectUserRegW();
}

void UserLoginWindow::slot_login_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS) {
        qDebug() << "network error" << err;
        return;
    }

    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()) {
        qDebug() << "json is null";
        return;
    }
    if(!jsonDoc.isObject()) {
        qDebug() << "parse json  error";
        return;
    }

    // 调用相应的映射函数
    m_handlers[id](jsonDoc.object());
}

void UserLoginWindow::slot_server_connect_finish(bool success)
{
    if(!success) {
        qDebug() << "connect to server ERROR! ";
        return;
    }
    qDebug() << "connect to server OK! send login req ... ";

    // 向服务器发送登录请求
    QJsonObject obj;
    obj["uid"] = m_uid;
    obj["token"] = m_token;
    obj["device"] = this_device_name;

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Indented);

    emit TcpMgr::GetInstance().get()->sig_send_data(ReqId::ID_CHAT_LOGIN, data);


}

void UserLoginWindow::slot_switch_ChatW()
{

    MainWindow* parentW = qobject_cast<MainWindow*>(this->window());
    parentW->selectChatMainW();
}


