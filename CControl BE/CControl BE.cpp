// CControl BE.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Client.h"
#include "util\log.h"
#include "handler\handler.h"

#define ADDRESS     "192.168.85.132"
#define PORT		1883
#define CLIENTID    "CControlServer"
#define TOPIC       "CControl.Volume"
#define PAYLOAD     "70"
#define QOS         1
#define TIMEOUT     10000L

void addHandlers(Client &client) {
	auto volumeHandler = std::make_shared<VolumeHandler>();
	client.subscribeWithHandler(NULL, "CControl/Volume", 1,volumeHandler);
}

int main()
{
	//initialize logging
	init_logging();
	CControlLogger lg;
	
	//initialize mosquitto library
	mosqpp::lib_init();

	//Create mosquitto client
	Client client(CLIENTID);

	//connect to broker
	int rc = client.connect(ADDRESS, PORT);
	if (rc != MOSQ_ERR_SUCCESS) {
		BOOST_LOG_SEV(lg,ERROR) << mosqpp::strerror(rc);
		return 0;
	}
	//add message handlers
	addHandlers(client);

	// start main network loop
	client.loop_start();
	while (std::cin.get() != 'q')
		;
	client.loop_stop();
	//disconnect from broker
	client.disconnect();
	mosqpp::lib_cleanup();
    return 0;
}

