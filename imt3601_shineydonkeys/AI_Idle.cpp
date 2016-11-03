#include "AI_Idle.h"
#include "PhysicsComponent.h"
#include "AiComponent.h"
#include "AI_Patrol.h"

AiIdle::AiIdle(AiComponent* component) : AiState(component)
{
}

AiIdle::~AiIdle()
{
}

void AiIdle::update()
{

	//auto pc = _AiComponent->getParent().getComponent<PhysicsComponent>();


	_AiComponent->ChangeState(new AiPatrol(_AiComponent));

	//pc->move();

}
