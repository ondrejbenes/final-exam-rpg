#pragma once
#include "EntityComponent.h"
#include "AI_State.h"

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
