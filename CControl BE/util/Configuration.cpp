#include "../stdafx.h"
#include "Configuration.h"
#include "log.h"

namespace pt = boost::property_tree;


Configuration::Configuration(std::string clientId,std::string hostname,int port) 
	: clientId(clientId),hostname(hostname),port(port)
{
}


std::shared_ptr<Configuration> Configuration::loadConfiguration()
{
	std::string clientId;
	std::string hostname;
	int port;
	
	pt::ptree propertyTree;
	std::ifstream file;
	file.open("CControl.cfg");
	try {
		pt::read_xml(file, propertyTree);
	}
	catch (pt::xml_parser_error& e) {
		throw std::runtime_error(e.what());
	}

	try {
		clientId = propertyTree.get<std::string>("clientid");
	}
	catch (pt::ptree_bad_data& e) {
		throw std::runtime_error("Invalid clientid supplied in configuration file...");
	}
	catch (pt::ptree_bad_path& e) {
		throw std::runtime_error("No clientid supplied in configuration file...");
	}
	hostname = propertyTree.get("hostname","localhost");
	port = propertyTree.get("port", 1883);

	std::shared_ptr<Configuration> config(new Configuration(clientId, hostname, port));
	return config;
}

const std::string& Configuration::getClientId()
{
	return clientId;
}

const std::string& Configuration::getHostname()
{
	return hostname;
}

const int& Configuration::getPort()
{
	return port;
}
