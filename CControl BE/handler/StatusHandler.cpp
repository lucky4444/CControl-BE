#include "../stdafx.h"
#include "StatusHandler.h"
#include "VolumeHandler.h"

namespace pt = boost::property_tree;

StatusHandler::StatusHandler(std::shared_ptr<Client> base) : cli(base)
{
	
}

void StatusHandler::handle(Message msg)
{
	if (msg.payload == "" || msg.payload == CLIENTID) {
		BOOST_LOG_SEV(lg, log_level::DEBUG) << "Received status request.";
		int volume = -1;
		try {
			volume = VolumeHandler::getVolume();
		}
		catch (std::runtime_error e) {
			BOOST_LOG_SEV(lg, log_level::ERR) << e.what();
		}
		std::stringstream stream;
		pt::ptree tree;
		tree.add("clientid", CLIENTID);
		tree.add("volume", volume);
		tree.add("timer", "0");
		try {
			pt::write_json(stream, tree, true);
		}
		catch (pt::json_parser_error e) {
			BOOST_LOG_SEV(lg, log_level::ERR) << "Failed to post status because of json parser error: " << e.what();
			return;
		}
		std::string payload = stream.str();
		size_t payloadlen = payload.length();
		cli->publish(NULL, "CControl/Status/Response", payloadlen, payload.c_str(), 1, false);
		BOOST_LOG_SEV(lg, log_level::DEBUG) << "Sent status response.";
	}
	else{
		BOOST_LOG_SEV(lg, log_level::DEBUG) << "Received status request for another client. Ignoring...";
	}
}


StatusHandler::~StatusHandler()
{
}
