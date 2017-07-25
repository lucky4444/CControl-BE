#pragma once
#include "../util/log.h"
#include "MessageHandler.h"
#include "Message.h"

class VolumeHandler : public MessageHandler
{
	CControlLogger lg;
	void updateVolume(int volume);
public:
	/*
	 * Called when another client wants to change the volume of the default audio device.
	 * The msg payload must contain the following information formatted as json:
	 * {
	 *		"clientid": "Lukas-PC",
	 *		"volume": 60
	 * }
	 * The client id is the name that the server used for  
	 * The volume value must be a positive integer between inclusive 0 and 100.
	 */
	void handle(Message msg) override;
};

