#pragma once
#include "EntityComponent.h"

// TODO remove DL etc after we have changed spritesheets for 4 directions
enum MovementDirection { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };

class PhysicsComponent : public EntityComponent
{
public:	
	/**
	 * \brief Default velocity in pixels per second
	 */
	static const sf::Vector2f defaultVelocity;

	explicit PhysicsComponent(Entity& parent);
	virtual ~PhysicsComponent();

	void update() override;

	sf::Vector2f getVelocity();
	void setVelocity(sf::Vector2f velocity);
private:
	sf::Vector2f velocity;
	sf::Clock sinceLastMove;
	bool hasCollision(const sf::Vector2f& newPosition);
	void move();
};