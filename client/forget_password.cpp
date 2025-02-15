#include "forget_password.h"
#include "ui_forget_password.h"
#include "httpmgr.h"
forget_password::forget_password(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::forget_password)
{
    ui->setupUi(this);
    connect(HttpMgr::GetInstance().get(),&HttpMgr::sig_http_frg_mod_finish,
            this,&forget_password::slot_reg_mod_finish);
    inithandle_respond();
}

forget_password::~forget_password()
{
    delete ui;
}

void forget_password::on_pushButton_4_clicked()
{
    emit switch_login_dia();
}


void forget_password::on_get_varefy_clicked()
{
    QString email = ui->email_edit->text();
    QRegularExpression emailRegex("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}");
    bool match = emailRegex.match(email).hasMatch();
    if(match){

        // 发送http验证码
        QJsonObject json_obj;
        json_obj["email"] = email;
        HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/get_varifycode"),
                                            json_obj,ReqId::Id_GET_VARIFY_CODE,Modules::FORGET_MOD);
    }else{
        QMessageBox::information(this,"错误","邮箱格式错误");
    }
}

void forget_password::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
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

void forget_password::inithandle_respond()
{
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
    _handlers.insert(ReqId::RESET_PASSWORD,[this](const QJsonObject& jsonObj){
        int error = jsonObj["error"].toInt();
        if(error == ErrorCodes::ERR_Varify){
            QMessageBox::information(this,"错误·","验证码错误");
            return;
        }
        if(error != ErrorCodes::SUCCESS){
            QMessageBox::information(this,"错误","参数错误");
            return;
        }
        auto email = jsonObj["email"].toString();
        QMessageBox::information(this,"成功","重置成功");
        //qDebug()<< "email is " << email ;
        //qDebug()<< "user uuid is " <<  jsonObj["uuid"].toString();
        QMessageBox::information(this,"重置为",jsonObj["passwd"].toString());
        emit switch_login_dia();

    });


}



void forget_password::on_yes_clicked()
{
    // 检查是否填写 完备
    if(ui->user_edit->text() == ""){
        QMessageBox::information(this,"提示·","用户名不能为空");
        return;
    }
    if(ui->email_edit->text() == ""){
        QMessageBox::information(this,"提示·","邮箱不能为空");
        return;
    }
    if(ui->new_password->text() == ""){
        QMessageBox::information(this,"提示·","新密码不能为空");
        return;
    }


    if(ui->varify_edit->text() == ""){
        QMessageBox::information(this,"提示·","验证码不能为空");
        return;
    }

    QJsonObject send;
    QJsonObject json_obj;
    json_obj["user"] = ui->user_edit->text();
    json_obj["email"] = ui->email_edit->text();
    json_obj["newpasswd"] = xorString(ui->new_password->text());
    json_obj["varifycode"] = ui->varify_edit->text();
    HttpMgr::GetInstance()->PostHttpReq(QUrl(gate_url_prefix+"/user_reset_password"),
                                        json_obj, ReqId::RESET_PASSWORD,Modules::FORGET_MOD);
}

