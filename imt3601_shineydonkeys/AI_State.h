#pragma once
#include "AiComponent.h"
class AiState 
{
public:

	explicit AiState(AiComponent* component);
	virtual ~AiState();
	virtual void update() = 0;
protected:
	AiComponent* _AiComponent;

};

