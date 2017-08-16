#pragma once
#include "Message.h"
#include "MessageHandler.h"
#include "../Client.h"
#include "ShutdownHandler.h"
class StatusHandler : public MessageHandler
{
	std::weak_ptr<Client> cli;
	std::shared_ptr<ShutdownHandler> shutdownHandler;
public:
	StatusHandler(std::weak_ptr<Client> base,std::shared_ptr<ShutdownHandler> sHandler);
	void handle(Message msg);
};

