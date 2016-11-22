#pragma once

#include "EntityComponent.h"

#include <SFML/Graphics/Sprite.hpp>

class AnimationComponent : public EntityComponent
{
public:
	explicit AnimationComponent(Entity& parent);

	void update() override;
private:
	sf::Vector2i spriteSheetCell;
	sf::Clock _animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;

	void doCombatAnimation();
	void doMoveAnimation();
};

