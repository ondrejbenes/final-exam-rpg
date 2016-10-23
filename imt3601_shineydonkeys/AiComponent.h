#pragma once
#include "EntityComponent.h"

class AiState;

class AiComponent : public EntityComponent
{

public:
	explicit AiComponent(Entity& parent);
	virtual ~AiComponent();
	void ChangeState(AiState* newState);

	void update() override;
private:
	AiState* CurrentState;
};
