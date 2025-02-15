#include "CSession.h"
#include "CServer.h"
#include "LogicSystem.h"
CSession::CSession(boost::asio::io_context& io_context, CServer* server) :
	_socket(io_context), _server(server), _b_close(false), _b_head_parse(false) {
	boost::uuids::uuid  a_uuid = boost::uuids::random_generator()();
	_session_id = boost::uuids::to_string(a_uuid);
	_recv_head_node = make_shared<MsgNode>(HEAD_TOTAL_LEN);
    
}
void CSession::Start() {
    AsyncReadHead(HEAD_TOTAL_LEN);
}
CSession::~CSession() {
	std::cout << "~CSession destruct" << endl;
}

tcp::socket& CSession::GetSocket() {
	return _socket;
}

std::string& CSession::GetSessionId() {
	return _session_id;
}
void CSession::SetUserId(int uid)
{
    _user_uid = uid;
}
void CSession::Close() {
    _socket.close();
    _b_close = true;
}
int CSession::GetUserId()
{
    return _user_uid;
}
void CSession::AsyncReadHead(int total_len) {

	boost::asio::async_read(_socket, boost::asio::buffer (_data), boost::asio::transfer_exactly(HEAD_TOTAL_LEN),
        [=](const boost::system::error_code& ec,int num) {
        try {
            if (ec) {
                std::cout << "handle read failed, error is " << ec.what() << endl;
                Close();
                _server->ClearSession(_session_id);
                return;
            }
            std::cout << num << std::endl;
            _recv_head_node->Clear();
            memcpy(_recv_head_node->_data, _data, HEAD_TOTAL_LEN);
            //��ȡͷ��MSGID����
            short msg_id = 0;
            memcpy(&msg_id, _recv_head_node->_data, HEAD_ID_LEN);
            //�����ֽ���ת��Ϊ�����ֽ���
            msg_id = boost::asio::detail::socket_ops::network_to_host_short(msg_id);
            std::cout << "msg_id is " << msg_id << endl;
            //id�Ƿ�
            if (msg_id > MAX_LENGTH) {
                std::cout << "invalid msg_id is " << msg_id << endl;
                _server->ClearSession(_session_id);
                return;
            }
            short msg_len = 0;
            memcpy(&msg_len, _recv_head_node->_data + HEAD_ID_LEN, HEAD_DATA_LEN);
            //�����ֽ���ת��Ϊ�����ֽ���
            msg_len = boost::asio::detail::socket_ops::network_to_host_short(msg_len);
            std::cout << "msg_len is " << msg_len << endl;
            //���ȷǷ�
            if (msg_len > MAX_LENGTH) {
                std::cout << "invalid data length is " << msg_len << endl;
                _server->ClearSession(_session_id);
                return;
            }
            _recv_msg_node = make_shared<RecvNode>(msg_len, msg_id);

            AsyncReadBody(msg_len);
        }
        catch (std::exception& e) {
            std::cout << "Exception code is " << e.what() << endl;
        }
    });
}
void CSession::AsyncReadBody(int total_len)
{
    auto self = shared_from_this();
    boost::asio::async_read(_socket, boost::asio::buffer(_data), boost::asio::transfer_exactly(total_len),
        [=](const boost::system::error_code& ec,int num){
        try {
            if (ec) {
                std::cout << "handle read failed, error is " << ec.what() << endl;
                Close();
                _server->ClearSession(_session_id);
                return;
            }

          
            
            _recv_msg_node->Clear();
            memcpy(_recv_msg_node->_data, _data, total_len);
            _recv_msg_node->_cur_len += total_len;
            _recv_msg_node->_data[_recv_msg_node->_total_len] = '\0';
            cout << "receive data is " << _recv_msg_node->_data << endl;
            //�˴�����ϢͶ�ݵ��߼�������
            LogicSystem::GetInstance()->PostMsgToQue(make_shared<LogicNode>(shared_from_this(), _recv_msg_node));
            //��������ͷ�������¼�
            AsyncReadHead(HEAD_TOTAL_LEN);
        }
        catch (std::exception& e) {
            std::cout << "Exception code is " << e.what() << endl;
        }
        });
}

LogicNode::LogicNode(shared_ptr<CSession>  session,
    shared_ptr<RecvNode> recvnode) :_session(session), _recvnode(recvnode) {

}


void CSession::Send(std::string msg, short msgid) {
    std::lock_guard<std::mutex> lock(_send_lock);
    int send_que_size = _send_que.size();
    if (send_que_size > MAX_SENDQUE) {
        std::cout << "session: " << _session_id << " send que fulled, size is " << MAX_SENDQUE << endl;
        return;
    }

    _send_que.push(make_shared<SendNode>(msg.c_str(), msg.length(), msgid));
    if (send_que_size > 0) {
        return;
    }
    auto& msgnode = _send_que.front();
    std::cout << msgnode->_data << std::endl;
    boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
        std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}

void CSession::Send(char* msg, short max_length, short msgid) {
    std::lock_guard<std::mutex> lock(_send_lock);
    int send_que_size = _send_que.size();
    if (send_que_size > MAX_SENDQUE) {
        std::cout << "session: " << _session_id << " send que fulled, size is " << MAX_SENDQUE << endl;
        return;
    }

    _send_que.push(make_shared<SendNode>(msg, max_length, msgid));
    if (send_que_size > 0) {
        return;
    }
    auto& msgnode = _send_que.front();
    boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
        std::bind(&CSession::HandleWrite, this, std::placeholders::_1, SharedSelf()));
}


std::shared_ptr<CSession>CSession::SharedSelf() {
    return shared_from_this();
}


void CSession::HandleWrite(const boost::system::error_code& error, std::shared_ptr<CSession> shared_self) {
    //�����쳣����
    try {
        if (!error) {
            std::lock_guard<std::mutex> lock(_send_lock);
            //cout << "send data " << _send_que.front()->_data+HEAD_LENGTH << endl;
            _send_que.pop();
            if (!_send_que.empty()) {
                auto& msgnode = _send_que.front();
                boost::asio::async_write(_socket, boost::asio::buffer(msgnode->_data, msgnode->_total_len),
                    std::bind(&CSession::HandleWrite, this, std::placeholders::_1, shared_self));
            }
        }
        else {
            std::cout << "handle write failed, error is " << error.what() << endl;
            Close();
            _server->ClearSession(_session_id);
        }
    }
    catch (std::exception& e) {
        std::cerr << "Exception code : " << e.what() << endl;
    }

}