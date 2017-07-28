#include "../stdafx.h"
#include "StatusHandler.h"
#include "VolumeHandler.h"
#include "../util/log.h"

namespace pt = boost::property_tree;

StatusHandler::StatusHandler(std::shared_ptr<Client> client) : cli(client)
{
	
}

void StatusHandler::handle(Message msg)
{
	if (msg.payload == "" || msg.payload == CLIENTID) {
		LOG_DEBUG << "Received status request.";
		int volume = -1;
		try {
			volume = VolumeHandler::getVolume();
		}
		catch (std::runtime_error e) {
			LOG_ERROR << e.what();
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
			LOG_ERROR << "Failed to post status because of json parser error: " << e.what();
			return;
		}
		std::string payload = stream.str();
		size_t payloadlen = payload.length();
		cli->publish(NULL, "CControl/Status/Response", payloadlen, payload.c_str(), 1, false);
		LOG_DEBUG << "Sent status response.";
	}
	else{
		LOG_DEBUG << "Received status request for another client. Ignoring...";
	}
}
