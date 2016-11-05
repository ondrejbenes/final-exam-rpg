#include "AiAttack.h"
#include "AiComponent.h"
#include "PhysicsComponent.h"
#include "AiPatrol.h"
#include "EntityManager.h"
#include "VectorUtilities.h"
#include "Logger.h"

AiAttack::AiAttack(AiComponent* component, sf::Vector2f center, float attackRadius) : 
AiState(component),
_center(center),
_attackRadius(attackRadius)
{

}

AiAttack::~AiAttack()
{

} 

void AiAttack::update()
{
	auto playerPos = EntityManager::getInstance()->getLocalPlayer()->getPosition();
	auto aiPos = _aiComponent->getParent().getPosition();

	if (isPlayerInRadius(_center, _attackRadius))
	{
		if (VectorUtilities::calculateDistance(playerPos, aiPos) > 100)
			setVelocityTowardsPosition(playerPos);
		else
		{
			LOG_D("Starting combat");
			_aiComponent->getParent().getComponent<PhysicsComponent>()->setVelocity(sf::Vector2f(0, 0));
		}
	} 
	else
	{
		_aiComponent->ChangeState(new AiPatrol(_aiComponent, _center, _attackRadius));
		LOG_D("Going from Attack to Patrol");
	}

}
