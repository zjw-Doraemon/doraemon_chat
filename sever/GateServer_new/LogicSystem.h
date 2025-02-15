#pragma once
#include "const.h"

class HttpConnection;
using HttpHandler = std::function<void(std::shared_ptr<HttpConnection>)>;
class LogicSystem :public Singleton<LogicSystem>
{
	friend class Singleton<LogicSystem>;
public:
	~LogicSystem();
	bool HandleGet(std::string,std::shared_ptr<HttpConnection>);
	void RegGet(std::string url, HttpHandler handler);
	void RegPost(std::string url, HttpHandler handler);	
	bool HandlePost(std::string, std::shared_ptr<HttpConnection>);

private:
	LogicSystem();
	std::map<std::string, HttpHandler> _getHandlers;
	std::map<std::string, HttpHandler> _postHandlers;
};

