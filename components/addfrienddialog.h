#ifndef ADDFRIENDDIALOG_H
#define ADDFRIENDDIALOG_H

#include <QDialog>
#include "global.h"
#include "usermgr.h"

namespace Ui {
class AddFriendDialog;
}

class AddFriendDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(std::shared_ptr<UserInfo> userInfo READ userInfo WRITE setUserInfo NOTIFY userInfoChanged FINAL)
public:
    explicit AddFriendDialog(QWidget *parent = nullptr);
    AddFriendDialog(std::shared_ptr<UserInfo> userInfo);
    ~AddFriendDialog();

    std::shared_ptr<UserInfo> userInfo() const;
    void setUserInfo(const std::shared_ptr<UserInfo> &newUserInfo);

signals:
    void userInfoChanged();

private slots:
    void on_cancel_lb_clicked(QMouseEvent *);

    void on_confirm_lb_clicked(QMouseEvent *);

private:
    Ui::AddFriendDialog *ui;

    std::shared_ptr<UserInfo> m_userInfo;


};

#endif // ADDFRIENDDIALOG_H
