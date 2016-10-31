#pragma once
#include "AI_State.h"
class AiFollow :
	public AiState
{
public:
	AiFollow(AiComponent* component);
	~AiFollow();
	void update() override;
private:
	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;
	int test = 0;
};

