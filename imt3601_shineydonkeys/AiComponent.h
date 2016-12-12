#pragma once
#include "EntityComponent.h"

class AiState;

class AiComponent : public EntityComponent
{

public:
	explicit AiComponent(Entity& parent);
	void ChangeState(AiState* newState);

	void update() override;
private:
	AiState* CurrentState;
};
