#pragma once
#include "EntityComponent.h"

class PhysicsComponent : public EntityComponent
{

public:
	explicit PhysicsComponent(Entity& parent);
	virtual ~PhysicsComponent();

	void update() override;

private:
	sf::Vector2f velocity;

};