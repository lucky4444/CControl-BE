#pragma once
struct Message
{
	int id;
	std::string topic;
	int qos;
	std::string payload;
	int payloadlen;
	bool retain;
public:
	Message(const mosquitto_message* msg);
};

