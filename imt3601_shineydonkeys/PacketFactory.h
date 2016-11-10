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
	LEAVE_COMBAT,
	TAKE_DAMAGE
};

class PacketFactory
{
public:
	sf::Packet createGameStarted();
	sf::Packet createGameOver();

	sf::Packet createVelocityChange(unsigned int entityId, const sf::Vector2f& newVelocity);
	sf::Packet createPositionChange(unsigned int entityId, const sf::Vector2f& newPosition);
	//sf::Packet createStatsChange(unsigned int entityId, const CharacterStats& stats);
	
	sf::Packet createEnterCombat(unsigned int entityId);
	sf::Packet createLeaveCombat(unsigned int entityId);
	sf::Packet createTakeDamage(unsigned int entityId, unsigned int damage);

	const static char ATTRIBUTE_SEPARATOR = ';';
};

