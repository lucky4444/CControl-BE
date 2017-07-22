#include "stdafx.h"
#include <iostream>
#include <unordered_map>
#include "MessageHandler.h"
#include "Client.h"

Client::Client(const char * id, bool clean_session) : mosqpp::mosquittopp(id,clean_session)
{
	std::string name(id);
	log(INFO,"Client " + name +  " initialized");
}

Client::~Client()
{
	log(INFO,"Client destroyed");
}

int Client::subscribe(int * mid, const char * topic, int qos, std::shared_ptr<MessageHandler> handler)
{
	int rc = subscribe(mid, topic, qos);

	return 0;
}

void Client::log(log_level level,std::string message)
{
	BOOST_LOG_SEV(lg, level) << message;
}

void Client::on_connect(int rc)
{
	log(INFO,"connected");
}

void Client::on_message(const mosquitto_message * message)
{
	log(DEBUG,(char *)message->payload);
}

void Client::on_log(int level, const char * str)
{
	log_level l = DEBUG;
	switch (level){
	case MOSQ_LOG_INFO:
		l = INFO;
		break;
	case MOSQ_LOG_ERR:
		l = ERROR;
		break;
	case MOSQ_LOG_NOTICE:
		l = NOTICE;
		break;
	case MOSQ_LOG_DEBUG:
		l = DEBUG;
		break;
	case MOSQ_LOG_WARNING:
		l = WARNING;
		break;
	}
	log(l,str);
}

void Client::on_error()
{
	log(ERROR,"An error occured");
}
