#pragma once
#include "Message.h"
#include "MessageHandler.h"
#include "../Client.h"
#include "ShutdownHandler.h"
class StatusHandler : public MessageHandler
{
	std::string CLIENTID;
	std::weak_ptr<Client> cli;
	std::shared_ptr<ShutdownHandler> shutdownHandler;
public:
	StatusHandler(std::shared_ptr<Client> client,std::shared_ptr<ShutdownHandler> sHandler);
	void handle(Message msg);
};

