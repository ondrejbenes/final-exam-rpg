#pragma once
#include "EntityComponent.h"

// TODO remove DL etc after we have changed spritesheets for 4 directions
enum MovementDirection { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };

class PhysicsComponent : public EntityComponent
{
public:	
	static const sf::Vector2f defaultVelocity;

	explicit PhysicsComponent(Entity& parent);
	virtual ~PhysicsComponent();

	void update() override;

	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);

	// TODO remove method, move centents to update?
	void move();
private:
	sf::Vector2f velocity;
	bool hasCollision(const sf::Vector2f& newPosition);
};