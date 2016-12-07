#pragma once

#include "AiComponent.h"

class AiState 
{
public:
	explicit AiState(AiComponent* component);
	virtual void update() = 0;
protected:
	AiComponent* _aiComponent;

	void setVelocityTowardsPosition(const sf::Vector2f& position);
	bool isPlayerInRadius(const sf::Vector2f& center, const float radius) const;
};