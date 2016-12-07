#pragma once

#include "EntityComponent.h"
#include "Trigger.h"

#include <SFML/Graphics/Rect.hpp>
#include "Character.h"
#include "Tile.h"

enum MovementDirection { Down, Left, Right, Up };

class PhysicsComponent : public EntityComponent
{
public:	
	static const std::string MOVE_SPRITE_NAME;

	/* Default NPC velocity in pixels per second */
	static const sf::Vector2f DEFAULT_NPC_VELOCITY;

	/* Default Player velocity in pixels per second */
	static const sf::Vector2f DEFAULT_PLAYER_VELOCITY;

	/* Zero (still) velocity */
	static const sf::Vector2f ZERO_VELOCITY;

	explicit PhysicsComponent(Entity& parent, bool _static = false);

	void update() override;

	MovementDirection getDirection() const { return _direction; }

	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);

	void setCollider(sf::FloatRect collider) { _collider = collider; }
	sf::FloatRect& getCollider() { return _collider; }

	std::vector<std::shared_ptr<Trigger>>& getTriggers() { return _triggers; }
private:
	bool _static;
	
	MovementDirection _direction;
	sf::Vector2f _velocity;
	sf::Clock _sinceLastMove;
	sf::FloatRect _collider;

	std::vector<std::shared_ptr<Trigger>> _triggers;

	bool hasCollision(const sf::Vector2f& newPosition, const std::vector<Tile*>& tilesToCheck, const std::vector<Character*>& charactersToCheck) const;
	void checkTriggers(const sf::Vector2f& newPosition, const std::vector<Tile*>& tilesToCheck, const std::vector<Character*>& charactersToCheck) const;
	void move();
};