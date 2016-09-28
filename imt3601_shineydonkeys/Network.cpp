#include "Network.h"


Network::Network() : Module(NETWORK)
{

}



Network::~Network()
{

}

bool Network::initialize()
{

	return true;
}


int Network::broadcast(sf::Packet& packet)
{

	return 0;
}


sf::Packet Network::receive()
{
	sf::Packet packet;
	return packet;
}