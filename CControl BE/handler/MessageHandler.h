#pragma once
#include "Message.h"

class MessageHandler
{
public:
	virtual void handle(Message message)
	{
		throw std::runtime_error("MessageHandler is an interface and therefore cannot be accessed directly.");
	}
};

