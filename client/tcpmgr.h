#ifndef TCPMGR_H
#define TCPMGR_H
#include <QTcpSocket>
#include "singleton.h"
#include "global.h"
#include <QObject>
#include <functional>
class TcpMgr:public QObject, public Singleton<TcpMgr>,
               public std::enable_shared_from_this<TcpMgr> // qt不一定需要
{
    Q_OBJECT
public:
    ~TcpMgr();
    QTcpSocket _socket;   // 暂时公开

private:
    TcpMgr();
    friend class Singleton<TcpMgr>;
    void initHandlers();
    void handleMsg(ReqId id, int len, QByteArray data);

    QString _host;
    uint16_t _port;
    QByteArray _buffer;
    bool _b_recv_pending;
    quint16 _message_id;
    quint16 _message_len;
     QMap<ReqId, std::function<void(ReqId id, int len, QByteArray data)>> _handlers;
public slots:
    //void slot_tcp_connect(ServerInfo);
    void slot_send_data(ReqId reqId, QByteArray data);
    void slot_tcp_connect(ServerInfo si);

signals:
    void sig_con_success(bool bsuccess);
    void sig_send_data(ReqId reqId, QByteArray data);
    void sig_swich_chatdlg();
    void sig_login_failed(int);
    void sig_log_success();

private:

};

#endif // TCPMGR_H
