#pragma once
#include "../util/log.h"
#include "MessageHandler.h"
#include "Message.h"

class VolumeHandler : public MessageHandler
{
	CControlLogger lg;
public:
	VolumeHandler();
	void handle(Message msg) override;
	~VolumeHandler();
};

