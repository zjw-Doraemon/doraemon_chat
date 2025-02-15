
#pragma once
#include <boost/asio.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast.hpp>
#include <boost/asio.hpp>
#include <queue>
#include <mutex>
#include <memory>
//#include "const.h"
#include "MsgNode.h"
#include <iostream>
using namespace std;
namespace beast = boost::beast;         // from <boost/beast.hpp>
namespace http = beast::http;           // from <boost/beast/http.hpp>
namespace net = boost::asio;            // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;       // from <boost/asio/ip/tcp.hpp>

class CServer;
class LogicSystem;


class CSession : public std::enable_shared_from_this<CSession>
{
public:
	CSession(boost::asio::io_context& io_context, CServer* server);
	~CSession();
	void SetUserId(int uid); // 绑定用户id
	int GetUserId();
	tcp::socket& GetSocket();
	std::string& GetSessionId();
	void CSession::Start();
	void CSession::AsyncReadHead(int);
	void AsyncReadBody(int length);
	void Close();
	void Send(char* msg, short max_length, short msgid);
	void Send(std::string msg, short msgid);
	void HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self);
	std::shared_ptr<CSession> SharedSelf();

private:
	tcp::socket _socket;
	std::string _session_id;
	
	bool _b_close;
	char _data[MAX_LENGTH];
	CServer* _server;
	//收到的消息结构
	std::shared_ptr<RecvNode> _recv_msg_node;
	bool _b_head_parse;
	//收到的头部结构
	std::shared_ptr<MsgNode> _recv_head_node;
	int _user_uid;


	std::mutex _send_lock;
	std::queue<shared_ptr<SendNode> > _send_que;
};


class LogicNode {
	friend class LogicSystem;
public:
	LogicNode(shared_ptr<CSession>, shared_ptr<RecvNode>);
private:
	shared_ptr<CSession> _session;
	shared_ptr<RecvNode> _recvnode;

};