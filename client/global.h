#ifndef GLOBAL_H
#define GLOBAL_H

#include<QWidget>
#include<functional>
#include"QStyle"
#include<iostream>
#include<memory>
#include<mutex>
#include<QNetworkReply>
#include<QMessageBox>
#include<QJsonObject>
#include<QDir>
#include<QSettings>




enum ReqId{
    Id_GET_VARIFY_CODE = 1001,//获取验证码
    ID_REG_USER = 1002,// 注册用户
    RESET_PASSWORD =1003, //重置密码
    ID_LOGIN_USER  = 1004,// 登录获取聊天服务器地址
    ID_CHAT_LOGIN = 1005, //登陆聊天服务器
    ID_CHAT_LOGIN_RSP= 1006, //登陆聊天服务器回包
    ID_SEARCH_USER_REQ = 1007, //用户搜索请求
    ID_SEARCH_USER_RSP = 1008, //搜索用户回包
    ID_ADD_FRIEND_REQ = 1009,  //添加好友申请
    ID_ADD_FRIEND_RSP = 1010, //申请添加好友回复
    ID_NOTIFY_ADD_FRIEND_REQ = 1011,  //通知用户添加好友申请
    ID_AUTH_FRIEND_REQ = 1013,  //认证好友请求
    ID_AUTH_FRIEND_RSP = 1014,  //认证好友回复
    ID_NOTIFY_AUTH_FRIEND_REQ = 1015, //通知用户认证好友申请
    ID_TEXT_CHAT_MSG_REQ  = 1017,  //文本聊天信息请求
    ID_TEXT_CHAT_MSG_RSP  = 1018,  //文本聊天信息回复
    ID_NOTIFY_TEXT_CHAT_MSG_REQ = 1019, //通知用户文本聊天信息

};

enum Modules{
    REGISTERMOD = 0,
    FORGET_MOD = 1,
    LOGINMOD = 2
};
enum ErrorCodes{

    SUCCESS = 0,
    ERR_JSON = 1, // json解析失败
    ERR_NETWORK = 2,// 网络错误
    ERR_Varify = 1004, //验证码错误
    VarifyExpired = 1003, //验证码过期
};
struct ServerInfo{
    QString Host;
    QString Port;
    QString Token;
    int Uid;
};

extern QString gate_url_prefix;

extern std::function<QString(QString)> xorString;


#endif // GLOBAL_H
