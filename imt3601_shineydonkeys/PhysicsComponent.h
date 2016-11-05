#pragma once
#include "EntityComponent.h"

// TODO remove DL etc after we have changed spritesheets for 4 directions
enum MovementDirection { Down, Left, Right, Up };

class PhysicsComponent : public EntityComponent
{
public:	
	/* Default velocity in pixels per second */
	static const sf::Vector2f defaultVelocity;

	/* Zero (still) velocity */
	static const sf::Vector2f ZERO_VELOCITY;

	explicit PhysicsComponent(Entity& parent);
	virtual ~PhysicsComponent();

	void update() override;

	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);
private:
	sf::Vector2f velocity;
	sf::Clock sinceLastMove;
	bool hasCollision(const sf::Vector2f& newPosition) const;
	void move();
};