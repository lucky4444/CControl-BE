#include "stdafx.h"
#include "handler\Message.h"
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

int Client::subscribeWithHandler(int * mid, const char * topic, int qos, std::shared_ptr<MessageHandler> handler)
{
	int rc = subscribe(mid, topic, qos);
	if (rc == MOSQ_ERR_SUCCESS) {
		handlers[topic] = handler;
	}
	return rc;
}

int Client::unsubscribeHandler(int * mid, const char * topic)
{
	int rc = unsubscribe(mid,topic);
	if (rc == MOSQ_ERR_SUCCESS) {
		handlers.erase(topic);
	}
	return rc;
}

void Client::log(log_level level,std::string message)
{
	BOOST_LOG_SEV(lg, level) << message;
}

void Client::on_connect(int rc)
{
	std::stringstream stream;
	stream << "Connected to mqtt broker " << HOSTNAME;
	log(INFO,stream.str());
}

void Client::on_message(const mosquitto_message * message)
{
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
	log_level l = DEBUG;
	switch (level){
	case MOSQ_LOG_INFO:
		l = INFO;
		break;
	case MOSQ_LOG_ERR:
		l = ERR;
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
	//log(l,str);
}

void Client::on_error()
{
	log(ERR,"An error occured");
}
