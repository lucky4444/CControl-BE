#pragma once
#include "../util/log.h"
#include "Message.h"
#include "MessageHandler.h"
class StatusHandler : public MessageHandler
{
	CControlLogger lg;
public:
	StatusHandler();
	void handle(Message msg);
	~StatusHandler();
};

