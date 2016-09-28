#pragma once
#include "Module.h"

#include <SFML/Network.hpp>

class Network : public Module
{

public:
	Network();
	virtual ~Network();
	bool initialize() override;

	int broadcast(sf::Packet& packet);
	sf::Packet receive();

private:
	sf::IpAddress serverIp;
	unsigned short serverPort;

};