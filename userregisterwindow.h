#ifndef USERREGISTERWINDOW_H
#define USERREGISTERWINDOW_H

#include <QWidget>
#include "global.h"
#include <iostream>
#include <functional>
#include <QMap>

namespace Ui {
class UserRegisterWindow;
}

class UserRegisterWindow : public QWidget
{
    Q_OBJECT

public:
    explicit UserRegisterWindow(QWidget *parent = nullptr);
    ~UserRegisterWindow();
    // 初始化HTTP请求映射表
    void initHttpHandlers();

public slots:
    void slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err);
private slots:
    void on_return_login_btn_clicked();

    void on_verify_btn_clicked();

    void on_confirm_reg_btn_clicked();

private:
    Ui::UserRegisterWindow *ui;

    QMap<ReqId, std::function<void(const QJsonObject&)>> m_handlers;
};

#endif // USERREGISTERWINDOW_H
