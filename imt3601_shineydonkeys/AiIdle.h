#pragma once

#include "AiState.h"

class AiIdle : public AiState
{
public:
	AiIdle(AiComponent* component);
	~AiIdle();
	void update() override;
private:
	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;
};

