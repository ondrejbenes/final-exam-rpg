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
	std::stringstream ss;
	ss << POSITION_CHANGE;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entityId;
	ss << ATTRIBUTE_SEPARATOR;
	ss << newPosition.x;
	ss << ATTRIBUTE_SEPARATOR;
	ss << newPosition.y;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}

sf::Packet PacketFactory::createEnterCombat(unsigned entity1Id, unsigned entity2Id)
{
	std::stringstream ss;
	ss << ENTER_COMBAT;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entity1Id;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entity2Id;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}

sf::Packet PacketFactory::createEndCombat(unsigned entityId) 
{
	std::stringstream ss;
	ss << END_COMBAT;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entityId;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}

sf::Packet PacketFactory::createTakeDamage(unsigned entityId, unsigned damage) 
{
	std::stringstream ss;
	ss << TAKE_DAMAGE;
	ss << ATTRIBUTE_SEPARATOR;
	ss << entityId;
	ss << ATTRIBUTE_SEPARATOR;
	ss << damage;

	sf::Packet packet;
	packet << ss.str();
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

sf::Packet PacketFactory::createChangeTileSprite(unsigned tileId, unsigned tileType, bool blocking) 
{
	std::stringstream ss;
	ss << TILE_SPRITE_CHANGE;
	ss << ATTRIBUTE_SEPARATOR;
	ss << tileId;
	ss << ATTRIBUTE_SEPARATOR;
	ss << tileType;
	ss << ATTRIBUTE_SEPARATOR;
	ss << blocking;

	sf::Packet packet;
	packet << ss.str();
	return packet;
}
