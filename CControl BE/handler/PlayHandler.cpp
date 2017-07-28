#include "../stdafx.h"
#include "PlayHandler.h"
#include "../util/log.h"
#include <Windows.h>

namespace pt = boost::property_tree;

void PlayHandler::performAction(std::string action)
{
	WORD key;
	if (action == "next") {
		key = VK_MEDIA_NEXT_TRACK;
	}
	else if (action == "prev") {
		key = VK_MEDIA_PREV_TRACK;
	}
	else if (action == "play_pause") {
		key = VK_MEDIA_PLAY_PAUSE;
	}
	else {
		std::stringstream ret;
		ret << "Failed to perform play change request. Invalid action: " << action;
		throw std::runtime_error(ret.str());
	}

	INPUT input;
	input.type = INPUT_KEYBOARD;
	input.ki.wScan = 0;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	input.ki.wVk = key;

	input.ki.dwFlags = 0;
	UINT rc;
	rc = SendInput(1, &input, sizeof(input));
	
	if (rc == 0) {
		LOG_ERROR << "Failed to insert event for key down.";
	}
	else {
		LOG_DEBUG << "Inserted " << rc << " event(s) for key down.";
	}

	input.ki.dwFlags = KEYEVENTF_KEYUP;
	rc = SendInput(1, &input, sizeof(input));
	if (rc == 0) {
		LOG_ERROR << "Failed to insert event for key up.";
	}
	else {
		LOG_DEBUG << "Inserted " << rc << " event(s) for key up.";
	}
}

void PlayHandler::handle(Message msg)
{
	LOG_DEBUG << "Play change request received.";
	pt::ptree payload;
	std::stringstream stream;
	stream << msg.payload;
	try {
		pt::read_json(stream, payload);
	}
	catch (pt::json_parser_error e) {
		LOG_ERROR << "Failed to perform play change request because request contains invalid json.";
		return;
	}

	std::string action = payload.get("action", "");
	std::string clientid = payload.get("clientid", "");
	if (clientid != CLIENTID) {
		LOG_DEBUG << "Received play change request is for different client. Ignoring it...";
		return;
	}

	try {
		performAction(action);
	}
	catch (std::runtime_error e) {
		LOG_ERROR << e.what();
		return;
	}

	if (action == "next") {
		LOG_DEBUG << "Jumped to next song";
	}
	else if (action == "prev") {
		LOG_DEBUG << "Jumped to previous song";
	}
	else if (action == "play_pause") {
		LOG_DEBUG << "Changed Play/Pause state";
	}	
}
