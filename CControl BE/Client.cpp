#include "stdafx.h"
#include "Client.h"
#include "handler\Message.h"
#include "util\log.h"

Client::Client(const std::string& id, bool clean_session) 
	: clientid_(id),mosqpp::mosquittopp(id.c_str(),clean_session)
{
	LOG_INFO << "Client [" << (std::string)id << "] initialized";
}

Client::~Client()
{
	LOG_INFO << "Client destroyed";
}

int Client::subscribeWithHandler(int * mid, const char * topic, int qos, std::shared_ptr<MessageHandler> handler)
{
	int rc = subscribe(mid, topic, qos);
	if (rc == MOSQ_ERR_SUCCESS) {
		handlers[topic] = handler;
		LOG_DEBUG << "Suscribed to topic " << topic;
	}
	return rc;
}

int Client::unsubscribeHandler(int * mid, const char * topic)
{
	int rc = unsubscribe(mid,topic);
	if (rc == MOSQ_ERR_SUCCESS) {
		handlers.erase(topic);
		LOG_DEBUG << "Suscribed from topic " << topic;
	}
	return rc;
}

const std::string& Client::getId()
{
	return clientid_;
}

void Client::on_connect(int rc)
{
	//LOG_INFO << "Connected to mqtt broker " << HOSTNAME;
}

void Client::on_message(const mosquitto_message * message)
{
	LOG_DEBUG << "Received message for topic " << message->topic;
	std::unique_lock<std::mutex> guard(messageMutex,std::defer_lock);
	guard.lock();
	Message msg(message);
	auto handler = handlers.find(msg.topic);
	if (handler != handlers.end()) {
		handler->second->handle(msg);
	}
	guard.unlock();
}

void Client::on_log(int level, const char * str)
{
	switch (level){
	case MOSQ_LOG_INFO:
		LOG_INFO << str;
		break;
	case MOSQ_LOG_ERR:
		LOG_ERROR << str;
		break;
	case MOSQ_LOG_NOTICE:
		LOG_INFO << str;
		break;
	/*case MOSQ_LOG_DEBUG:
		LOG_DEBUG << str;
		break;*/
	case MOSQ_LOG_WARNING:
		LOG_WARNING << str;
		break;
	}
}

void Client::on_error()
{
	LOG_ERROR << "An error occured";
}

void Client::shutdown()
{
	InitiateShutdown(NULL, L"CControl Client requested a shutdown",
		0, SHUTDOWN_FORCE_OTHERS | SHUTDOWN_POWEROFF, SHTDN_REASON_MAJOR_APPLICATION);
}
