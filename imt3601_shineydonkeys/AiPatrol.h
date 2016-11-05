#pragma once

#include "AiState.h"

class AiPatrol : public AiState
{
public:
	AiPatrol(AiComponent* component, sf::Vector2f center, float patrolRadius);
	~AiPatrol();
	void update() override;
private:
	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;
	int test = 0; // for testing

	sf::Clock _timeSpentAtPoint;
	sf::Vector2f _center;
	float _patrolRadius;
	sf::Vector2f _movePoint;

	sf::Vector2f getNextMovePoint() const;
};

