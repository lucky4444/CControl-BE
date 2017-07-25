// CControl BE.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Client.h"
#include "util\log.h"
#include "handler\handler.h"

void addHandlers(std::shared_ptr<Client> client) {
	//set volume
	auto volumeHandler = std::make_shared<VolumeHandler>();
	client->subscribeWithHandler(NULL, "CControl/Volume", 1,volumeHandler);

	//post Client name,Volume and Timer
	std::shared_ptr<StatusHandler> statusHandler(new StatusHandler(client));
	client->subscribeWithHandler(NULL, "CControl/Status/Request", 1, statusHandler);

	//start timer, reset timer
	/*auto shutdownHandler = std::make_shared<ShutdownHandler>();
	client.subscribeWithHandler(NULL, "CControl/Shutdown", 1, volumeHandler);*/

	//stop,play,next song, previous song
	auto playHandler = std::make_shared<PlayHandler>();
	client->subscribeWithHandler(NULL, "CControl/Play", 1, playHandler);
}

int main()
{
	//initialize logging
	init_logging(true);
	CControlLogger lg;
	
	//initialize mosquitto library
	mosqpp::lib_init();

	//Create mosquitto client
	std::shared_ptr<Client> client(new Client(CLIENTID));

	//connect to broker
	int rc = client->connect(HOSTNAME, PORT);
	if (rc != MOSQ_ERR_SUCCESS) {
		BOOST_LOG_SEV(lg,log_level::ERR) << "Couldn't establish connection to mqtt broker: " << mosqpp::strerror(rc);
		return 0;
	}
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

