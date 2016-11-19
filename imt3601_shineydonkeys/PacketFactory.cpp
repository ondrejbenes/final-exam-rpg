#include "PacketFactory.h"
#include "Logger.h"
#include <sstream>

sf::Packet PacketFactory::createGameStarted() {
	sf::Packet packet;
	packet << std::to_string(GAME_START);

	std::string msg;
	packet >> msg;
	LOG_D(msg);

	return packet;
}

sf::Packet PacketFactory::createGameOver() 
{
	sf::Packet packet;
	packet << std::to_string(GAME_OVER);

	return packet;
}

sf::Packet PacketFactory::createVelocityChange(unsigned entityId, const sf::Vector2f& newVelocity) 
{
	std::stringstream ss;
	ss << VELOCITY_CHANGE;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entityId;
	ss << ATTRIBUTE_SEPARATOR;
	ss << newVelocity.x;
	ss << ATTRIBUTE_SEPARATOR;
	ss << newVelocity.y;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}

sf::Packet PacketFactory::createPositionChange(unsigned entityId, const sf::Vector2f& newPosition) 
{
	sf::Packet packet;
	packet << POSITION_CHANGE;

	return packet;
}

sf::Packet PacketFactory::createEnterCombat(unsigned entityId) 
{
	sf::Packet packet;
	packet << ENTER_COMBAT;

	return packet;
}

sf::Packet PacketFactory::createLeaveCombat(unsigned entityId) 
{
	sf::Packet packet;
	packet << LEAVE_COMBAT;

	return packet;
}

sf::Packet PacketFactory::createTakeDamage(unsigned entityId, unsigned damage) 
{
	sf::Packet packet;
	packet << TAKE_DAMAGE;

	return packet;
}

sf::Packet PacketFactory::createChatMessage(const std::string& message)
{
	std::stringstream ss;
	ss << CHAT;
	ss << ATTRIBUTE_SEPARATOR;
	ss << message;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}
