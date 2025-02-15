#include "register_dialog.h"
#include "ui_register_dialog.h"
#include <QRegularExpressionMatch>
#include "httpmgr.h"
register_dialog::register_dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::register_dialog),
    eye(new QAction(QIcon(":/res/unvisible.png"), "Clear", this)),
    eye_2(new QAction(QIcon(":/res/unvisible.png"), "Clear", this))
{
    ui->setupUi(this);
    setFixedSize(262,350);
    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    connect(ui->getVerificationCodeButton,&QPushButton::clicked, this,&register_dialog::get_verify_code);
    //qDebug ()<< "创建注册窗口";
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_http_reg_mod_finish,
            this,&register_dialog::slot_reg_mod_finish);
    initHttpHandlers();
    // 密码显示 与隐藏
    ui->pass_edit->addAction(eye,QLineEdit::TrailingPosition);
    ui->confirm_edit->addAction(eye_2,QLineEdit::TrailingPosition);
    connect(eye,&QAction::triggered,this,[this]{
        if(QLineEdit::Normal == ui->pass_edit->echoMode()){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
            this->eye->setIcon(QIcon(":/res/unvisible.png"));
        }
        else if(QLineEdit::Password == ui->pass_edit->echoMode()){
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
            this->eye->setIcon(QIcon(":/res/visible.png"));
        }



    });
    connect(eye_2,&QAction::triggered,this,[this]{
        if(QLineEdit::Normal == ui->confirm_edit->echoMode()){
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
            this->eye_2->setIcon(QIcon(":/res/unvisible.png"));
        }
        else if(QLineEdit::Password == ui->confirm_edit->echoMode()){
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
            this->eye_2->setIcon(QIcon(":/res/visible.png"));
        }
    });

}

register_dialog::~register_dialog()
{
    delete ui;
    //qDebug()<<"注册窗口销毁了";
}

void register_dialog::get_verify_code()
{
    QString email = ui->email_edit->text();
    QRegularExpression emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    bool match = emailRegex.match(email).hasMatch();
    if(match){
        ui->error_tip->setText(" ");
        // 发送http验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                            json_obj,ReqId::Id_GET_VARIFY_CODE,Modules::REGISTERMOD);
    }else{
        ui->error_tip->setText("邮箱错误");
        ui->error_tip->setStyleSheet("color:red");
    }
}

void register_dialog::initHttpHandlers()
{
    //注册获取验证码
    _handlers.insert(ReqId::Id_GET_VARIFY_CODE,[this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error != ErrorCodes::SUCCESS){
            QMessageBox::information(this,"错误","参数错误");
            return;
        }
        //auto email = jsonObj["email"].toString();
        QMessageBox::information(this,"提示","验证码已经发送到邮箱 注意查收");
        //qDebug() <<"email is"<<email;
    });
    _handlers.insert(ReqId::ID_REG_USER, [this](QJsonObject jsonObj){
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::ERR_Varify){
            showTip(tr("验证码错误"),false);
            return;
        }
        if(error != ErrorCodes::SUCCESS){
            showTip(tr("参数错误"),false);
            return;
        }


        auto email = jsonObj["email"].toString();
        showTip(tr("用户注册成功"), true);

        qDebug()<<"user uuid is " << jsonObj["uid"].toString();
        qDebug()<< "email is " << email ;
    });
}



void register_dialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
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

void register_dialog::on_sure_btu_clicked()
{
    if(ui->user_edit->text() == ""){
        showTip(tr("用户名不能为空"), false);
        return;
    }
    if(ui->email_edit->text() == ""){
        showTip(tr("邮箱不能为空"), false);
        return;
    }
    if(ui->pass_edit->text() == ""){
        showTip(tr("密码不能为空"), false);
        return;
    }
    if(ui->confirm_edit->text() == ""){
        showTip(tr("确认密码不能为空"), false);
        return;
    }
    if(ui->confirm_edit->text() != ui->pass_edit->text()){
        showTip(tr("密码和确认密码不匹配"), false);
        return;
    }
    if(ui->varify_edit->text() == ""){
        showTip(tr("验证码不能为空"), false);
        return;
    }
    //day11 发送http请求注册用户
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["passwd"] = xorString(ui->pass_edit->text());
    json_obj["confirm"] = xorString(ui->confirm_edit->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_register"),
                                        json_obj, ReqId::ID_REG_USER,Modules::REGISTERMOD);
}

void register_dialog::showTip(QString content, bool)
{
     QMessageBox::information(this,"提示",content);
}

void register_dialog::on_cancel_btn_clicked()
{
    emit show_mainwindows();
}

