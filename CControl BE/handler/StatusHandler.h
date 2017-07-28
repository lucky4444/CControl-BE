#pragma once
#include "Message.h"
#include "MessageHandler.h"
#include "../Client.h"
class StatusHandler : public MessageHandler
{
	std::shared_ptr<Client> cli;
public:
	StatusHandler(std::shared_ptr<Client> base);
	void handle(Message msg);
};

