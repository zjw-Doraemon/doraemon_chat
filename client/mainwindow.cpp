#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , _login_dialog(new LoginDialog())
    , _forget_widget(new forget_password())

{
    ui->setupUi(this);
    // 设置固定大小
    setFixedSize(262,462);
    setCentralWidget(_login_dialog); //这个函数自动管理 传入的窗口 ，包括销毁
    // setcentralwidget 切换下一个窗口时 上一个窗口会被销毁   dialog 默认 按下 esc键关闭
    connect(_login_dialog,&LoginDialog::swichRegister,this,[&]{
        register_dialog register_dig; // 这个时候才创建 注册窗口
        connect(&register_dig,&register_dialog::show_mainwindows,this,[&]{
            this->show();
            register_dig.close();
        });
        this->hide();
        register_dig.exec();
    });
    // 切换忘记密码窗口 与主窗口 互相切换
    connect(_login_dialog,&LoginDialog::switch_foreget_widget,this,[&]{
        _login_dialog = static_cast<LoginDialog*>(takeCentralWidget());
        setCentralWidget(_forget_widget);

    });
    connect(_forget_widget,&forget_password::switch_login_dia,this,[&]{
        _forget_widget = static_cast<forget_password*>(takeCentralWidget());
        setCentralWidget(_login_dialog);

    });





}

MainWindow::~MainWindow()
{
    delete ui;
}
