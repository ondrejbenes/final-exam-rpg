#pragma once
#include "EntityComponent.h"

class AiComponent : public EntityComponent
{

public:
	explicit AiComponent(Entity& parent);
	virtual ~AiComponent();

	void update() override;

	// TODO remove duplicity w/ playerinput
private:
	sf::Vector2i* spriteSheetCell;
	sf::Clock animationTimer;
	static sf::Time ANIMATION_PERIOD_MS;

	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;
};