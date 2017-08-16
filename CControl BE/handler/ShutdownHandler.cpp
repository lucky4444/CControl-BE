#include "../stdafx.h"
#include "ShutdownHandler.h"
#include "../util/log.h"

namespace pt = boost::property_tree;

ShutdownHandler::ShutdownHandler(std::weak_ptr<Client> client) : cli(client),timer(ioservice)
{
	timer.expires_from_now(boost::posix_time::seconds(0));
	ioservice.run();
}

void ShutdownHandler::handle(Message msg)
{
	LOG_DEBUG << "Shutdown request received.";
	pt::ptree payload;
	std::stringstream stream;
	stream << msg.payload;
	try {
		pt::read_json(stream, payload);
	}
	catch (pt::json_parser_error e) {
		LOG_ERROR << "Failed to process shutdown request because request contains invalid json.";
		return;
	}
	std::string clientid = payload.get("clientid", "");
	std::string action = payload.get("action", "");
	int time = payload.get("time", -1);

	if (clientid != CLIENTID) {
		LOG_DEBUG << "Received shutdown request is for different client. Ignoring it...";
		return;
	}
	try {
		if (action == "set") {
			setTimer(time);
		}else if (action == "reset"){
			resetTimer();
		}else {
			LOG_ERROR << "Unknown action supplied for shutdown handler";
		}
	} catch (std::runtime_error e) {
		LOG_ERROR << e.what();
		return;
	}
}

int ShutdownHandler::getDuration()
{
	int duration = timer.expires_from_now().total_seconds();
	return (duration > 0) ? duration : -1;
}


void ShutdownHandler::setTimer(int t)
{
	if (t < 0 || t > ShutdownHandler::MAXLENGTH) {
		std::stringstream ss;
		ss << "Supplied time must be between 0 and " << ShutdownHandler::MAXLENGTH << " seconds.";
		throw std::runtime_error(ss.str());
	}

	timer.expires_from_now(boost::posix_time::seconds(t));
	timerThread.join();

	timer.async_wait(boost::bind(&ShutdownHandler::shutdown, this,boost::asio::placeholders::error));

	if (ioservice.stopped()) {
		ioservice.reset();
		timerThread = boost::thread(boost::bind(&boost::asio::io_service::run, &ioservice));
		LOG_DEBUG << "Set shutdown timer. " << t << " seconds left till shutdown.";
	} else {
		LOG_ERROR << "Cannot start shutdown timer because ioservice is not stopped";
	}
}

void ShutdownHandler::resetTimer()
{
	timer.expires_from_now(boost::posix_time::seconds(0));
	timerThread.join();
	LOG_INFO << "Shutdown timer canceled";
}

void ShutdownHandler::shutdown(const boost::system::error_code& err)
{
	if (err && err == boost::asio::error::operation_aborted) {
		LOG_DEBUG << "Shutdown handler called with operation aborted error";
	} else if ( err && err != boost::asio::error::operation_aborted) {
		LOG_ERROR << "Error occured while trying to shutdown the computer: " << err.message();
	} else {
		LOG_INFO << "Shutting down the computer ...";
		InitiateShutdown(NULL, L"CControl Client requested a shutdown",
			0, SHUTDOWN_FORCE_OTHERS | SHUTDOWN_POWEROFF, SHTDN_REASON_MAJOR_APPLICATION);
	}
}