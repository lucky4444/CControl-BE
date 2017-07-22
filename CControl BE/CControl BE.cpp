// CControl BE.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//

#include "stdafx.h"
#include "Client.h"
#include <iostream>
#include <string>
#include <memory.h>

#define ADDRESS     "192.168.85.132"
#define PORT		1883
#define CLIENTID    "CControlServer"
#define TOPIC       "CControl.Volume"
#define PAYLOAD     "70"
#define QOS         1
#define TIMEOUT     10000L

void init() {
	
	
}

int main()
{
	init_logging();
	boost::log::sources::severity_logger<log_level> lg;
	
	mosqpp::lib_init();
	Client client(CLIENTID);
	int rc = client.connect(ADDRESS, PORT);
	if (rc != MOSQ_ERR_SUCCESS) {
		BOOST_LOG_SEV(lg,ERROR) << mosqpp::strerror(rc);
	}
	client.subscribe(NULL, "Test", 1);
	client.loop_start();
	while (std::cin.get() != 'q')
		;
	client.disconnect();
	mosqpp::lib_cleanup();
    return 0;
}

