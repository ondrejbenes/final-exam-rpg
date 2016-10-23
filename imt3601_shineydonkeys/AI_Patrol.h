#pragma once
#include "AI_State.h"
class AiPatrol :
	public AiState
{
public:
	AiPatrol(AiComponent& component);
	~AiPatrol();
	void update() override;
private:
	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;

};

