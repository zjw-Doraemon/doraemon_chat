#include "loaddlg.h"
#include "ui_loaddlg.h"
#include "qmovie.h"
LoadDlg::LoadDlg(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoadDlg)
{
    ui->setupUi(this);

    setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowStaysOnTopHint);
    setAttribute(Qt::WA_TranslucentBackground); // 设置背景透明
    // 获取屏幕尺寸
    setFixedSize(parent->size()); // 设置对话框为全屏尺寸

    QMovie *movie = new QMovie(":/res/loading.gif"); // 加载动画的资源文件
    ui->loading_lb->setMovie(movie); // label 析构 movie 最后
    movie->start();
}

LoadDlg::~LoadDlg()
{
    delete ui;
}
