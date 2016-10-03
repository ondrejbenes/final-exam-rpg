#pragma once
#include "EntityComponent.h"

// TODO remove DL etc after we have changed spritesheets for 4 directions
enum MovementDirection { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };

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

