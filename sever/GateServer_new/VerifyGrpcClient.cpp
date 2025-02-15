#include "VerifyGrpcClient.h"
#include "ConfigMgr.h"

RPConPool::RPConPool(size_t poolsize, std::string host, std::string port):
	poolSize_(poolsize),host_(host),port_(port),b_stop_(false)
{
	for (size_t i = 0; i < poolSize_; i++) {
		std::string tmp = host + ":" + port;
		std::shared_ptr<Channel> channel = grpc::CreateChannel(host+":"+port, grpc::InsecureChannelCredentials());
		std::cout << tmp << std::endl;
		connections_.push ( VarifyService::NewStub(channel)) ;  // 右值移动
	}  
}

RPConPool::~RPConPool () {  // 有隐患 已解决
	{
		std::lock_guard<std::mutex> lock(mutex_);
		Close();
	}
	// 等待所有线程退出  不再等待
	while (active_threads_.load() > 0) {
		std::this_thread::yield(); // 主动让出CPU
	}
	// 清理资源

	while (!connections_.empty()) {
		connections_.pop();
	}
}

void RPConPool::Close() {
	b_stop_ = true;
	cond_.notify_all();
}

std::unique_ptr<VarifyService::Stub>RPConPool::getConnection() {
	std::unique_lock<std::mutex> lock(mutex_);
	active_threads_++;  // 进入计数器+1
	cond_.wait(lock, [this] {
		if (b_stop_) {
			return true;
		}
		return !connections_.empty();
		});
	active_threads_--;  // 退出计数器-1
	if (b_stop_ == true) {
		return nullptr;
	}
	auto context = std::move(connections_.front());
	connections_.pop();
	return context;

}

void RPConPool::returnConnection(std::unique_ptr<VarifyService::Stub> context) {
	std::lock_guard<std::mutex> lock(mutex_);
	if (b_stop_) {
		return;
	}
	connections_.push(std::move(context));
	cond_.notify_one();
}

VerifyGrpcClient:: VerifyGrpcClient() {
	auto& gCfgMgr = ConfigMgr::Inst();
	std::string host = gCfgMgr["VarifyServer"]["Host"];
	std::cout << host << std::endl;
	std::string port = gCfgMgr["VarifyServer"]["Port"];
	pool_.reset(new RPConPool(5,host,port));


}