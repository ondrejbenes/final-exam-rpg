#include "PhysicsComponent.h"
#include "AiComponent.h"
#include "AiPatrol.h"
#include "AiIdle.h"

AiIdle::AiIdle(AiComponent* component) : AiState(component)
{

}

AiIdle::~AiIdle()
{

}

void AiIdle::update()
{

	//auto pc = _aiComponent->getParent().getComponent<PhysicsComponent>();


	// _aiComponent->ChangeState(new AiPatrol(_aiComponent));

	//pc->move();

}
