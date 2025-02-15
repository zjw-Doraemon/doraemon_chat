#include "CServer.h"
#include "AsioIOServicePool.h"
#include "UserMgr.h"
CServer::CServer(boost::asio::io_context& io_context, short port) :_io_context(io_context), _port(port),
_acceptor(io_context, tcp::endpoint(tcp::v4(), port))
{
    std::cout << "Server start success, listen on port : " << _port << std::endl;
    StartAccept();
}
CServer::~CServer() {
    cout << "Server destruct listen on port : " << _port << endl;
}
void CServer::StartAccept() {
    auto& io_context = AsioIOServicePool::GetInstance()->GetIOService();
    std::shared_ptr<CSession> new_session = std::make_shared<CSession>(io_context, this);
    _acceptor.async_accept(new_session->GetSocket(), std::bind(&CServer::HandleAccept, this, new_session, std::placeholders::_1));
}

void CServer::HandleAccept(std::shared_ptr<CSession> new_session, const boost::system::error_code& error) {
    if (!error) {
        new_session->Start();
        std::lock_guard<std::mutex> lock(_mutex);
        _sessions.insert(make_pair(new_session->GetSessionId(), new_session));
    }
    else {
        std::cout << "session accept failed, error is " << error.what() << std::endl;
    }
    StartAccept();
}

void CServer::ClearSession(std::string uuid) {

    if (_sessions.find(uuid) != _sessions.end()) {
        //�Ƴ��û���session�Ĺ���
        UserMgr::GetInstance()->RmvUserSession(_sessions[uuid]->GetUserId());
    }

    {
        lock_guard<mutex> lock(_mutex);
        _sessions.erase(uuid);
    }

}