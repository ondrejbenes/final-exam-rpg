#include "AiComponent.h"
#include "GraphicsComponent.h"
#include "AiPatrol.h"

AiComponent::AiComponent(Entity& parent) : 
EntityComponent(parent),
CurrentState(nullptr)
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
