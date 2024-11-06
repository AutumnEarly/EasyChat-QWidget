#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : FramelessMainWindow(parent)
    , ui(new Ui::MainWindow), m_userLoginW(std::make_shared<UserLoginWindow>()),
    m_userRegW(nullptr)
{

    ui->setupUi(this);
    QFile file(":/style/style.qss");
    if(file.open(QIODevice::ReadOnly)) {
        this->setStyleSheet(file.readAll());
    }
    else {
        qDebug() << "文件打开失败!";
    }

    ui->stackedWidget->addWidget(m_userLoginW.get());
    this->setTitleBar(ui->title_bar);
    this->selectUserLoginW();
    this->setResizeEnable(false);
    this->setPadding(10);

}

MainWindow::~MainWindow()
{
    qDebug() << "MainWindow delete";
    delete ui;
}

void MainWindow::selectUserRegW()
{
    if(m_userRegW) {
        ui->stackedWidget->removeWidget(m_userRegW);
        delete m_userRegW;
    }
    m_userRegW = new UserRegisterWindow();
    ui->stackedWidget->addWidget(m_userRegW);
    ui->stackedWidget->setCurrentWidget(m_userRegW);
    m_userRegW->show();
}

void MainWindow::selectUserLoginW()
{
    ui->stackedWidget->setCurrentWidget(m_userLoginW.get());
}

void MainWindow::selectChatMainW()
{
    m_chatMainW = std::make_shared<ChatMainWindow>();
    m_chatMainW->show();
    this->hide();
}




void MainWindow::on_quit_win_btn_clicked(QMouseEvent *)
{
    this->close();
}

