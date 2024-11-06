#include "userregisterwindow.h"
#include "ui_userregisterwindow.h"
#include "mainwindow.h"
#include "httpmgr.h"

UserRegisterWindow::UserRegisterWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UserRegisterWindow)
{
    ui->setupUi(this);
    initHttpHandlers();
}

UserRegisterWindow::~UserRegisterWindow()
{

    delete ui;
}

void UserRegisterWindow::initHttpHandlers()
{
    // 添加注册映射
    m_handlers.insert(ReqId::ID_REG_USER,[this](const QJsonObject& obj) {
        int error = obj["error"].toInt();
        if(error != ErrorCodes::SUCCESS) {
            qDebug() << "register user error!" << obj;
            return;
        }
        qDebug() <<  "register OK Info is "<< obj;
    });
    // 添加获取验证码映射
    m_handlers.insert(ReqId::ID_GET_VARIFY_CODE,[this](const QJsonObject& obj) {
        int error = obj["error"].toInt();
        if(error != ErrorCodes::SUCCESS) {
            qDebug() << "get verify code error!" << obj;
            return;
        }
        qDebug() <<  "get verify code OK Info is "<< obj;
    });
}


void UserRegisterWindow::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
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

void UserRegisterWindow::on_return_login_btn_clicked()
{
    // 切换为登录页面
    MainWindow* parentW = qobject_cast<MainWindow*>(this->window());
    parentW->selectUserLoginW();
}


void UserRegisterWindow::on_verify_btn_clicked()
{
    // 验证码请求
    qDebug() << "send verify code !";

    QJsonObject obj;
    obj["email"] = ui->email_input->text();

    // 绑定注册类型的Http请求完成信号
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,
            this,&UserRegisterWindow::slot_reg_mod_finish,Qt::UniqueConnection);

    // 发送请求
    HttpMgr::GetInstance()->PostHttpReq(gate_url_prefix+"/get_reg_verifycode",obj,
                                        ReqId::ID_GET_VARIFY_CODE,Modules::REGISTERMOD);

}


void UserRegisterWindow::on_confirm_reg_btn_clicked()
{
    // 注册请求
    qDebug() << "send user register info !";

    QJsonObject obj;
    obj["user"] = ui->nickName_input->text();
    obj["passwd"] = ui->pwd_input->text();
    obj["email"] = ui->email_input->text();
    obj["verifycode"] = ui->verify_input->text();

    // 绑定注册类型的Http请求完成信号
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_reg_mod_finish,
            this,&UserRegisterWindow::slot_reg_mod_finish,Qt::UniqueConnection);

    // 发送请求
    HttpMgr::GetInstance()->PostHttpReq(gate_url_prefix+"/user_register",obj,
                                        ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

