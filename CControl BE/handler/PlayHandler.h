#pragma once
#include "Message.h"
#include "MessageHandler.h"
#include "../Client.h"


class PlayHandler : public MessageHandler
{
	std::string CLIENTID;
	void performAction(std::string action);
public:

	PlayHandler(std::shared_ptr<Client> client);

	/*
	* Called when another client wants to switch to the next/previous song or pause/play the current song.
	* The msg payload must contain the following information formatted as json:
	* {
	*		"clientid": "Lukas-PC",
	*		"action": "next"
	* }
	* The client id is the name that the server used for
	* The action value specifies the action to perform. Possible values are: 
	* next: jump to the next song
	* prev: jump to the previous song
	* play_pause: pause or continue the current song
	*/
	void handle(Message msg) override;
};

