#include "AiComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "AI_Patrol.h"
#include "AI_Idle.h"

AiComponent::AiComponent(Entity& parent) : 
EntityComponent(parent)
{
	CurrentState = new AiIdle(this);
}

AiComponent::~AiComponent()
{
	
}

void AiComponent::ChangeState(AiState* newState)
{
	CurrentState = newState;
}

void AiComponent::update()
{
	CurrentState->update();
}
