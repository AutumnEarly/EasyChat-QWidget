#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "userloginwindow.h"
#include "userregisterwindow.h"
#include "chatmainwindow.h"

#include "framelesscore/framelessmainwindow.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public FramelessMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    // 切换注册页
    void selectUserRegW();
    // 切换登录页
    void selectUserLoginW();
    // 切换到聊天窗口
    void selectChatMainW();
private slots:

    void on_quit_win_btn_clicked(QMouseEvent *);

private:
    Ui::MainWindow *ui;
    std::shared_ptr<UserLoginWindow> m_userLoginW;
    UserRegisterWindow *m_userRegW;
    std::shared_ptr<ChatMainWindow> m_chatMainW;
};
#endif // MAINWINDOW_H
