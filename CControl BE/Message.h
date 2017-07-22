#pragma once
class Message
{
	int id;
	std::string topic;
	int qos;
	std::string payload;
	int payloadlen;
	bool retain;
public:
	Message(mosquitto_message* msg);
};

