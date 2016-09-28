#pragma once
#include "EntityComponent.h"

class PlayerInputComponent : public EntityComponent
{
	enum Direction { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };
	sf::Vector2i source;
public:
	explicit PlayerInputComponent(Entity& parent);
	virtual ~PlayerInputComponent();

	void update() override;

};