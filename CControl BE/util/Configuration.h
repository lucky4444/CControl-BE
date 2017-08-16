#pragma once
class Configuration
{
private:
	Configuration(std::string clientId, std::string host,int port);
	const std::string hostname = "192.168.85.134";
	const int port = 1883;
	const std::string clientId = "CControlServer";
public:
	static std::shared_ptr<Configuration> loadConfiguration();
	const std::string& getHostname();
	const int& getPort();
	const std::string& getClientId();
};

