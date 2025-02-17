#include "mainwindow.h"
#include "QFile"
#include <QApplication>
#include "global.h"
/***********************************************
 * @file        mainwindow.h
 * @brief       主窗口
 *
 * @author      Doraemon
 * @date        2025
 * @version     1.0
 * @history     初始版本
 ***********************************************/



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // 加载 qss 文件
    QFile qss(":/style/stylesheet.qss");
    if(qss.open(QFile::ReadOnly)){
        QString style = QLatin1String(qss.readAll());
        a.setStyleSheet(style);
        qss.close();

    }else{
        qDebug()<<"open failed";
    }

    // 获取当前应用程序的路径
    QString app_path = QCoreApplication::applicationDirPath();

    // 拼接文件名
    QString fileName = "config.ini";
    QString config_path = QDir::toNativeSeparators(app_path +QDir::separator() + fileName);

    // 检查文件是否存在
    QFile file(config_path);
    if (!file.exists()) {
        qDebug() << "Config file does not exist: " << config_path;
        return 0 ;
    }

    QSettings settings(config_path, QSettings::IniFormat);
    QString gate_host = settings.value("GateServer/host").toString();
    QString gate_port = settings.value("GateServer/port").toString();
    gate_url_prefix = "http://"+gate_host+":"+gate_port;
    qDebug()<<gate_host<<gate_port;
    qDebug()<<gate_url_prefix;
    // 设置高dpi支持
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    a.setWindowIcon(QIcon(":/ico/icon.ico"));
    MainWindow w;
    w.show();
    return a.exec();
}
