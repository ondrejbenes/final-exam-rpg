#pragma once
#include "Module.h"

#include <SFML/Network.hpp>
#include <memory>
#include "PacketFactory.h"

class Network : public Module
{

public:
	static unsigned int PORT;

	Network();

	static bool isServer() { return _isServer; }
	static bool isMultiplayer() { return _isMultiplayer; }

	bool initialize() override;
	void update() override;

	void clear();
	void initAsServer();
	void initAsClient();

	void broadcast(sf::Packet& packet);

	sf::IpAddress getServerIp() const { return _serverIp; }
	void setServerIp(const sf::IpAddress& serverIp)
	{
		this->_serverIp = serverIp;
	}

private:
	static bool _isServer;
	static bool _isMultiplayer;

	// Client fields
	sf::TcpSocket _socket;
	sf::IpAddress _serverIp;

	// Server fields
	sf::TcpListener _listener;
	// TODO unique ptr?
	std::vector<sf::TcpSocket*> _clients; 
	sf::SocketSelector _selector;

	sf::Clock _refreshClock;
	sf::Time _refreshRate = sf::milliseconds(250);

	void updateClientMenu();
	void updateServerMenu();

	void updateClientMainGame();
	void updateServerMainGame();

	PacketType decodeMessage(std::string msg);
};
