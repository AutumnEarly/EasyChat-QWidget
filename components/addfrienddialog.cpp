#include "addfrienddialog.h"
#include "ui_addfrienddialog.h"

#include "tcpmgr.h"

AddFriendDialog::AddFriendDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddFriendDialog)
{
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->setupUi(this);
    auto setStyle = [&](QWidget* w, const QString& url) {
        QFile file(url);
        if(file.open(QIODevice::ReadOnly)) {
            w->setStyleSheet(file.readAll());
        }
        else {
            qDebug() << "文件打开失败!";
        }
    };

    setStyle(this, ":/style/add_friend/add_friend_dialog.qss");

}

AddFriendDialog::AddFriendDialog(std::shared_ptr<UserInfo> userInfo) : m_userInfo(userInfo)
{
    ui->name_input->setPlaceholderText(UserMgr::GetInstance()->getName());
}

AddFriendDialog::~AddFriendDialog()
{
    delete ui;
}

void AddFriendDialog::on_cancel_lb_clicked(QMouseEvent *)
{
    this->close();
    this->deleteLater();
}


void AddFriendDialog::on_confirm_lb_clicked(QMouseEvent *)
{

    QString name = ui->name_input->text();
    QString backName = ui->backName_input->text();
    QString label = ui->label_input->text();

    QJsonObject obj;
    obj["uid"] = UserMgr::GetInstance()->getUid();
    obj["touid"] = m_userInfo->_uid;
    obj["applyname"] = name;
    obj["nickname"] = backName;
    obj["tag"] = label;
    obj["descr"] = "备注信息";
    obj["device"] = this_device_name;

    QJsonDocument doc(obj);
    QByteArray data = doc.toJson(QJsonDocument::Compact);
    qDebug() << "addFriend info is name: " << name << " backName: " << backName << " lables: " << label;

    emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_ADD_FRIEND_REQ ,data);

    this->close();
    this->deleteLater();
}

std::shared_ptr<UserInfo> AddFriendDialog::userInfo() const
{
    return m_userInfo;
}

void AddFriendDialog::setUserInfo(const std::shared_ptr<UserInfo> &newUserInfo)
{
    if (m_userInfo == newUserInfo)
        return;
    m_userInfo = newUserInfo;

    ui->name_input->setText(UserMgr::GetInstance()->getName());

    emit userInfoChanged();
}

