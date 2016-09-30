#pragma once
#include "EntityComponent.h"

class PlayerInputComponent : public EntityComponent
{
public:
	explicit PlayerInputComponent(Entity& parent);
	virtual ~PlayerInputComponent();

	void update() override;

private:
	sf::Vector2i* spriteSheetCell;
	sf::Clock animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;
};