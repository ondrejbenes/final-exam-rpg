#pragma once

#include <SFML/Network/Packet.hpp>
#include <SFML/System/Vector2.hpp>

enum PacketType
{
	UNKNOWN = -1,
	GAME_START,
	GAME_OVER,
	VELOCITY_CHANGE,
	POSITION_CHANGE,
	STATS_CHANGE,
	ENTER_COMBAT,
	END_COMBAT,
	TAKE_DAMAGE,
	CHAT,
	TILE_SPRITE_CHANGE
};

class PacketFactory
{
public:
	sf::Packet createGameStarted();
	sf::Packet createGameOver();

	sf::Packet createVelocityChange(unsigned int entityId, const sf::Vector2f& newVelocity);
	sf::Packet createPositionChange(unsigned int entityId, const sf::Vector2f& newPosition);
	
	sf::Packet createEnterCombat(unsigned int entityId, unsigned entity2Id);
	sf::Packet createEndCombat(unsigned int entityId);
	sf::Packet createTakeDamage(unsigned int entityId, unsigned int damage);

	sf::Packet createChatMessage(const std::string& message);

	sf::Packet createChangeTileSprite(unsigned int tileId, unsigned int tileType, bool blocking);

	const static char ATTRIBUTE_SEPARATOR = ';';
};

