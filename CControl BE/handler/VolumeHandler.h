#pragma once
#include "MessageHandler.h"
#include "Message.h"
#include "../Client.h"

class VolumeHandler : public MessageHandler
{
private:
	std::string CLIENTID;
public:

	VolumeHandler(std::shared_ptr<Client> client);

	/*
	 * Set the master volume to the specified value.
	 * param volume - volume to set in percentage (0-100)
	 */
	static void updateVolume(int volume);

	/*
	 * Get the current master volume in percentage (0-100)
	 */
	static int getVolume();

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

