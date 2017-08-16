// CControl BE.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Client.h"
#include "util\log.h"
#include "util\Configuration.h"
#include "handler.h"

void addHandlers(std::shared_ptr<Client> client) {
	//set volume
	auto volumeHandler = std::make_shared<VolumeHandler>(client);
	client->subscribeWithHandler(NULL, "CControl/Volume", 1,volumeHandler);

	//start timer, reset timer
	auto shutdownHandler = std::make_shared<ShutdownHandler>(client);
	client->subscribeWithHandler(NULL, "CControl/Shutdown", 1, shutdownHandler);

	//stop,play,next song, previous song
	auto playHandler = std::make_shared<PlayHandler>(client);
	client->subscribeWithHandler(NULL, "CControl/Play", 1, playHandler);

	//post Client name,Volume and Timer
	auto statusHandler = std::make_shared<StatusHandler>(client,shutdownHandler);
	client->subscribeWithHandler(NULL, "CControl/Status/Request", 1, statusHandler);
}

int main()
{	
	std::shared_ptr<Configuration> config;
	try {
		config = Configuration::loadConfiguration();
	}
	catch (std::runtime_error& e) {
		LOG_ERROR << "Failed to read configuration file: " << e.what();
		return 0;
	}
	
	//initialize mosquitto library
	mosqpp::lib_init();

	//Create mosquitto client
	std::shared_ptr<Client> client = std::make_shared<Client>(config->getClientId());

	//connect to broker
	LOG_INFO << "Connecting to mqtt broker " << config->getHostname() << "...";
	int rc = client->connect(config->getHostname().c_str(), config->getPort());
	if (rc != MOSQ_ERR_SUCCESS) {
		LOG_ERROR << "Couldn't establish connection to mqtt broker: " << mosqpp::strerror(rc);
		return 0;
	}
	LOG_INFO << "Connection established";
	//add message handlers
	addHandlers(client);

	// start main network loop
	client->loop_start();
	while (std::cin.get() != 'q'){
		;
	}
	//disconnect from broker
	client->disconnect();

	client->loop_stop();
	mosqpp::lib_cleanup();
    return 0;
}

