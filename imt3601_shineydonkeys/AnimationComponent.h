#pragma once

#include "EntityComponent.h"

#include <SFML/Graphics/Sprite.hpp>

class AnimationComponent : public EntityComponent
{
public:
	explicit AnimationComponent(Entity& parent);
	virtual ~AnimationComponent();

	void update() override;
private:
	sf::Vector2i spriteSheetCell;
	unsigned int _spriteWidth;
	unsigned int _spriteHeight;
	sf::Sprite& _sprite;
	sf::Clock _animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;
};

