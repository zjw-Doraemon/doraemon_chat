#include "logindialog.h"
#include "ui_logindialog.h"
#include "httpMgr.h"
#include "tcpmgr.h"
LoginDialog::LoginDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->sign,&QPushButton::clicked,this,[=]{
        //qDebug() << " 登录窗口 注册信号触发";
        emit swichRegister();
    });
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_http_log_mod_finish,
            this,&LoginDialog::log_mod_finish);
    //连接tcp连接请求的信号和槽函数
    connect(this, &LoginDialog::sig_connect_tcp, TcpMgr::GetInstance().get(), &TcpMgr::slot_tcp_connect);
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_con_success, this, &LoginDialog::slot_tcp_con_finish);
    //连接tcp管理者发出的登陆失败信号
    connect(TcpMgr::GetInstance().get(), &TcpMgr::sig_login_failed, this, &LoginDialog::slot_login_failed);
    connect(TcpMgr::GetInstance().get(),&TcpMgr::sig_log_success,this,[=]{
        QMessageBox::information(this,"提示","登录成功");
    });
    inithandle_respond();
}

LoginDialog::~LoginDialog()
{
    delete ui;
    qDebug()<<"登录窗口销毁了";
}

void LoginDialog::on_forget_clicked()
{
    emit switch_foreget_widget();
}


void LoginDialog::on_login_clicked()
{
    TcpMgr::GetInstance()->_socket.close();
    //qDebug()<<"login btn clicked";
    if(ui->email_edit->text() == ""){
        QMessageBox::information(this,"提示","邮箱不能为空");
        return;
    }
    if(ui->key_word->text() == ""){
        QMessageBox::information(this,"提示","密码不能为空");
        return;
    }
    auto email = ui->email_edit->text();
    auto pwd = ui->key_word->text();
    //发送http请求登录
    QJsonObject json_obj;

    json_obj["email"] = email;
    json_obj["passwd"] = xorString(pwd);
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_login"),
                                        json_obj, ReqId::ID_LOGIN_USER,Modules::LOGINMOD);
}


void LoginDialog::log_mod_finish(ReqId id, QString res, ErrorCodes err)
{
    if(err != ErrorCodes::SUCCESS){
        QMessageBox::information(this,"错误","网络请求错误");
        return;
    }
    //解析json字符串 res 转化为 QByteArray
    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
    if(jsonDoc.isNull()){
        QMessageBox::information(this,"错误","json解析失败");
    }
    //json 解析错误
    if(!jsonDoc.isObject()){
        QMessageBox::information(this,"错误","json解析失败");
    }

    //jsonDoc.object();
    _handlers[id](jsonDoc.object());
    return;
}

void LoginDialog::inithandle_respond(){
    //注册获取登录回包逻辑
    _handlers.insert(ReqId::ID_LOGIN_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);

            return;
        }
        auto email = jsonObj["email"].toString();

        //发送信号通知tcpMgr发送长链接
        ServerInfo si;
        si.Uid = jsonObj["uid"].toInt();
        si.Host = jsonObj["host"].toString();
        si.Port = jsonObj["port"].toString();
        si.Token = jsonObj["token"].toString();

        _uid = si.Uid;
        _token = si.Token;
        qDebug()<< "email is " << email << " uid is " << si.Uid <<" host is "
                 << si.Host << " Port is " << si.Port << " Token is " << si.Token;
        emit sig_connect_tcp(si);
    });
}


void LoginDialog::slot_tcp_con_finish(bool bsuccess)
{

    if(bsuccess){
        qDebug()<<"聊天服务连接成功，正在登录...";
        QJsonObject jsonObj;
        jsonObj["uid"] = _uid;
        jsonObj["token"] = _token;

        QJsonDocument doc(jsonObj);
        QByteArray jsonData = doc.toJson(QJsonDocument::Indented);

        //发送tcp请求给chat server
        emit TcpMgr::GetInstance()->sig_send_data(ReqId::ID_CHAT_LOGIN, jsonData);

    }else{
        showTip(tr("网络异常"),false);

    }

}

void LoginDialog::showTip(QString content, bool)
{
     QMessageBox::information(this,"提示",content);
}

void LoginDialog::slot_login_failed(int err)
{
    QString result = QString("登录失败, err is %1")
                         .arg(err);
    showTip(result,false);

}
