#include "stdafx.h"
#include "Message.h"
#include "mosquitto.h"
#include <sstream>


Message::Message(mosquitto_message * msg)
	: id(msg->mid),topic(msg->topic),retain(msg->retain),qos(msg->qos)
{
	std::stringstream ss;
	int len = msg->payloadlen;
	for (int i = 0; i < len; i++) {
		ss << ((char *)msg->payload)[i];
	};
	this->payload = ss.str();
	this->payloadlen = len;
}
