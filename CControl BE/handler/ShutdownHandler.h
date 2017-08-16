#pragma once
#include "MessageHandler.h"
#include "Message.h"
#include "../Client.h"
#include <chrono>
class ShutdownHandler : public MessageHandler
{
	// Maximum time in seconds that is allowed to set
	// (Equals one day)
	const int MAXLENGTH = 86400;
	std::weak_ptr<Client> cli;
	boost::asio::io_service ioservice;
	boost::asio::deadline_timer timer;
	boost::thread timerThread;
	void setTimer(int t);
	void resetTimer();
public:
	ShutdownHandler(std::weak_ptr<Client> client);
	void handle(Message msg) override;
	int getDuration();
	void shutdown(const boost::system::error_code& err);
};

