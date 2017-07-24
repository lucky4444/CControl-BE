#pragma once
#include "util\log.h"
#include "handler\MessageHandler.h"

class Client : public mosqpp::mosquittopp
{	
private:
	CControlLogger lg;
	std::unordered_map<std::string, std::shared_ptr<MessageHandler>> handlers;
public:
	Client(const char *id = NULL, bool clean_session = true);
	~Client();
	int subscribeWithHandler(int* mid, const char * topic, int qos, std::shared_ptr<MessageHandler> handler);
	int unsubscribeHandler(int* mid, const char * topic);
	void log(log_level,std::string message);
	void on_connect(int rc) override;
	void on_disconnect(int rc) override { return; }
	void on_publish(int mid) override { return; }
	void on_message(const struct mosquitto_message * message) override;
	void on_subscribe(int mid, int qos_count, const int * granted_qos) override { return; }
	void on_unsubscribe(int mid) override { return; }
	void on_log(int level, const char * str) override;
	void on_error() override;
};

