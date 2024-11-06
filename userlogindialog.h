#ifndef USERLOGINDIALOG_H
#define USERLOGINDIALOG_H

#include <QDialog>

namespace Ui {
class UserLoginDialog;
}

class UserLoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit UserLoginDialog(QWidget *parent = nullptr);
    ~UserLoginDialog();

private:
    Ui::UserLoginDialog *ui;
};

#endif // USERLOGINDIALOG_H
