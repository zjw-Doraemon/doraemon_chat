#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();

private:
    Ui::LoginDialog *ui;
    QMap<ReqId,std::function<void(const QJsonObject&)>> _handlers;
signals:
    void swichRegister();
    void switch_foreget_widget();
    void switch_chatwindow();
    void sig_connect_tcp(ServerInfo);

private slots:
    void on_forget_clicked();
    void log_mod_finish(ReqId id, QString res, ErrorCodes err);
    void inithandle_respond();
    void on_login_clicked();
    void slot_tcp_con_finish(bool bsuccess);
    void slot_login_failed(int);
    void showTip(QString,bool);



private:
    int _uid;
    QString _token;
};

#endif // LOGINDIALOG_H
