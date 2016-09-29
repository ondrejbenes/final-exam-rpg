#pragma once
#include "EntityComponent.h"

enum Direction { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };

class PlayerInputComponent : public EntityComponent
{
public:
	explicit PlayerInputComponent(Entity& parent);
	virtual ~PlayerInputComponent();

	void update() override;

private:
	sf::Clock animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;
};