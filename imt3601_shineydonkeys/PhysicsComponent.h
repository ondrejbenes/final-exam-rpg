#pragma once
#include "EntityComponent.h"
#include <SFML/Graphics/Rect.hpp>

// TODO remove DL etc after we have changed spritesheets for 4 directions
enum MovementDirection { Down, Left, Right, Up };

class PhysicsComponent : public EntityComponent
{
public:	
	/* Default velocity in pixels per second */
	static const sf::Vector2f defaultVelocity;

	/* Zero (still) velocity */
	static const sf::Vector2f ZERO_VELOCITY;

	explicit PhysicsComponent(Entity& parent, bool _static = false);
	virtual ~PhysicsComponent();

	void update() override;

	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);

	sf::FloatRect& getCollider() { return _collider; }
private:
	bool _static;
	
	sf::Vector2f _velocity;
	sf::Clock _sinceLastMove;
	sf::FloatRect _collider;

	bool hasCollision(const sf::Vector2f& newPosition) const;
	void move();
};