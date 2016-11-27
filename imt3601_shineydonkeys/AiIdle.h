#pragma once

#include "AiState.h"

class AiIdle : public AiState
{
public:
	static float UPDATE_FREQUENCY;

	AiIdle(AiComponent* component, float attackRadius);

	void update() override;
private:
	float _attackRadius;
	sf::Clock _timer;
};

