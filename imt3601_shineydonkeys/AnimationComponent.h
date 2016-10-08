#pragma once
#include "EntityComponent.h"
#include "PhysicsComponent.h"

class AnimationComponent : public EntityComponent
{
public:
	explicit AnimationComponent(Entity& parent);
	virtual ~AnimationComponent();

	void update() override;

	void animate(MovementDirection directtion);
private:
	sf::Vector2i* spriteSheetCell;
	sf::Clock animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;
};

