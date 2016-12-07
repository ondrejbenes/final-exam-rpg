#pragma once

#include "AiState.h"

class AiPatrol : public AiState
{
public:
	AiPatrol(AiComponent* component, sf::Vector2f center, float patrolRadius);

	void update() override;

	static float UPDATE_FREQUENCY;
private:
	static unsigned CHANGE_MOVE_POINT_DISTANCE;

	sf::Clock _timeSinceLastUpdate;

	sf::Vector2f _center;
	float _patrolRadius;
	sf::Vector2f _movePoint;
	sf::Vector2f getNextMovePoint() const;
};

