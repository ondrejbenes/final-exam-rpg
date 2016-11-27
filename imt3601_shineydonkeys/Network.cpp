#include "Network.h"
#include "GamePhaseManager.h"
#include "Menu.h"
#include "Blackboard.h"
#include "Logger.h"
#include "GamePhaseFactory.h"
#include "StringUtilities.h"
#include "PacketFactory.h"
#include "EntityManager.h"
#include "PhysicsComponent.h"
#include "ChatBoard.h"
#include "ConfigIO.h"

#include <sstream>
#include <regex>
#include "CombatComponent.h"

Network::Network() : 
Module(NETWORK)
{

}

bool Network::initialize()
{
	clear();

	return true;
}

void Network::update()
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);

	if (!_isMultiplayer)
		return;

	auto phase = GamePhaseManager::getInstance()->getCurrentPhase();

	// TODO Strategy patt?
	if(typeid(*phase) == typeid(Menu))
	{
		if (_isServer)
			updateServerMenu();
		else
			updateClientMenu();
	} 
	else
	{
		if (_isServer)
			updateServerMainGame();
		else
			updateClientMainGame();
	}
}

void Network::clear()
{
	_socket.disconnect();
	
	_selector.remove(_listener);
	for (auto s : _clients)
	{
		s->disconnect();
		_selector.remove(*s);		
	}

	_listener.close();
	_clients.clear();

	_isServer = false;
	_isMultiplayer = false;
}

void Network::initAsServer()
{
	LOG_I("Initializing as Server");

	_listener.listen(PORT);
	_selector.add(_listener);

	_isServer = true;
	_isMultiplayer = true;
}

void Network::initAsClient()
{
	LOG_I("Initializing as Client");

	auto res = _socket.connect(_serverIp, PORT, sf::seconds(5));
	if (res == sf::Socket::Error)
	{
		LOG_E("Failed to connect to server (possible timeout)");
		return;
	}
	_socket.setBlocking(false);

	sf::Packet packet;
	packet << ConfigIO::readString(L"player", L"name");
	_socket.send(packet);

	_isServer = false;
	_isMultiplayer = true;
}

void Network::broadcast(sf::Packet& packet)
{
	/*
	std::string msg;
	packet >> msg;
	LOG_D("Broadcasting: " + msg);
	*/

	if (_isServer)
	{
		for (auto i = 0; i < _clients.size(); i++)
			_clients[i]->send(packet);
	}
	else
		_socket.send(packet);
}

void Network::updateClientMenu()
{
	sf::Packet packet;
	if (_socket.receive(packet) != sf::Socket::Done)
		return;

	std::string msg;
	packet >> msg;

	auto packetType = decodeMessage(msg);

	if (packetType == GAME_START)
	{
		LOG_I("Game starting");
		GamePhaseFactory factory;
		GamePhaseManager::getInstance()->pushPhase(factory.createMainGame());
	}
}

void Network::updateServerMenu()
{
	if (_refreshClock.getElapsedTime() < _refreshRate)
		return;
	_refreshClock.restart();

	if (_selector.wait(sf::seconds(0.1)))
	{
		if (_selector.isReady(_listener))
		{
			auto socket = new sf::TcpSocket();
			_listener.accept(*socket);
			sf::Packet packet;
			std::string id;
			if (socket->receive(packet) == sf::Socket::Done)
				packet >> id;
			LOG_D(id);

			auto x = 510;
			auto y = 100 + 50 * _clients.size();

			UiElementFactory factory;
			auto element = factory.createLabel(L"", L"");
			element->setPosition(sf::Vector2f(x, y));
			element->setText(id);

			auto& ui = GamePhaseManager::getInstance()->getCurrentPhase()->getUi();
			ui.addElement(element);

			_clients.push_back(socket);
			_selector.add(*socket);
		}		
	}
}


void Network::updateClientMainGame()
{
	sf::Packet packet;
	if (_socket.receive(packet) != sf::Socket::Done)
		return;

	std::string msg;
	packet >> msg;

	LOG_D("Received: " + msg);
	
	auto packetType = decodeMessage(msg);

	if (packetType == GAME_OVER)
	{
		GamePhaseManager::getInstance()->popPhase();
	}

	if (packetType == VELOCITY_CHANGE)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto charater = EntityManager::getInstance()->getCharacterById(stoi(tokens[1]));
		auto velocity = sf::Vector2f(stof(tokens[2]), stof(tokens[3]));
		charater->getComponent<PhysicsComponent>()->setVelocity(velocity);
	}

	if (packetType == POSITION_CHANGE)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto entityManager = EntityManager::getInstance();
		auto charater = entityManager->getCharacterById(stoi(tokens[1]));
		auto position = sf::Vector2f(stof(tokens[2]), stof(tokens[3]));
		entityManager->move(charater, position);
		charater->setPosition(position);
	}

	if (packetType == ENTER_COMBAT)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto entityManager = EntityManager::getInstance();
		auto entity1 = entityManager->getCharacterById(stoi(tokens[1]));
		auto entity2 = entityManager->getCharacterById(stoi(tokens[2]));

		entity1->getComponent<CombatComponent>()->startCombat(entity2);
	}
	
	if (packetType == TAKE_DAMAGE)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto entityManager = EntityManager::getInstance();
		auto entity = entityManager->getCharacterById(stoi(tokens[1]));
		auto cc = entity->getComponent<CombatComponent>();
		cc->takeDamage(stoi(tokens[2]));
	}

	if (packetType == CHAT)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto chatBoard = dynamic_cast<ChatBoard*>(GamePhaseManager::getInstance()->getCurrentPhase()->getUi().getElementByName("chatBoard"));
		auto msgTokens = StringUtilities::split(tokens[1], ':');
		chatBoard->addMessage(msgTokens[0], msgTokens[1]);
	}

	if (packetType == TILE_SPRITE_CHANGE)
	{
		auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
		auto tile = EntityManager::getInstance()->getTileById(stoi(tokens[1]));
		tile->changeType(stoi(tokens[2]), stoi(tokens[3]));
	}
}

void Network::updateServerMainGame()
{
	if (_selector.wait(sf::milliseconds(1)))
	{
		if (!_selector.isReady(_listener))
		{
			for (auto i = 0; i < _clients.size(); i++)
			{
				if (_selector.isReady(*_clients[i]))
				{
					sf::Packet packet, sendPacket;
					if (_clients[i]->receive(packet) == sf::Socket::Done)
					{
						std::string msg;
						packet >> msg;
						sendPacket << msg;
						for (auto j = 0; j < _clients.size(); j++)
						{
							if (i != j)
								_clients[j]->send(sendPacket);
						}

						auto packetType = decodeMessage(msg);

						if (packetType == CHAT)
						{
							auto tokens = StringUtilities::split(msg, PacketFactory::ATTRIBUTE_SEPARATOR);
							auto chatBoard = dynamic_cast<ChatBoard*>(GamePhaseManager::getInstance()->getCurrentPhase()->getUi().getElementByName("chatBoard"));
							auto msgTokens = StringUtilities::split(tokens[1], ':');
							chatBoard->addMessage(msgTokens[0], msgTokens[1]);
						}
					}
				}
			}
		}
	}
}


PacketType Network::decodeMessage(std::string msg) {
	std::regex reg1("[[:d:]]+");
	std::regex reg2("[[:d:]]+;.*");
	std::smatch match;

	auto packetType = -1;
	if (regex_search(msg, match, reg1))
	{
		packetType = stoi(msg);
	}
	else if (regex_search(msg, match, reg2))
	{
		packetType = stoi(msg.substr(0, msg.find(';')));
	}
	else
	{
		LOG_D("Packet with unknown message: " + msg);
	}
	
	return PacketType(packetType);
}

unsigned int Network::PORT = 52249;
bool Network::_isServer = false;
bool Network::_isMultiplayer = false;
