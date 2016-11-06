#include "AiAttack.h"
#include "AiComponent.h"
#include "PhysicsComponent.h"
#include "AiPatrol.h"
#include "EntityManager.h"
#include "VectorUtilities.h"
#include "Logger.h"
#include "CombatComponent.h"
#include "EntityFactory.h"

AiAttack::AiAttack(AiComponent* component, sf::Vector2f center, float attackRadius) :
AiState(component),
_center(center),
_attackRadius(attackRadius), 
_otherCombatComp(nullptr)
{
	_parentCombatComp = _aiComponent->getParent().getComponent<CombatComponent>();
}

AiAttack::~AiAttack()
{

} 

void AiAttack::update()
{
	auto player = EntityManager::getInstance()->getLocalPlayer();
	auto playerPos = player->getPosition();
	auto aiPos = _aiComponent->getParent().getPosition();

	if (isPlayerInRadius(_center, _attackRadius))
	{
		if (VectorUtilities::calculateDistance(playerPos, aiPos) > 100)
			setVelocityTowardsPosition(playerPos);
		else
		{
			if (!_parentCombatComp->isInCombat())
			{
				_parentCombatComp->startCombat(player);
				if (_otherCombatComp == nullptr)
					_otherCombatComp = player->getComponent<CombatComponent>();
				if (!_otherCombatComp->isInCombat())
				{
					auto parentAsChar = dynamic_cast<Character*>(&_aiComponent->getParent());
					_otherCombatComp->startCombat(parentAsChar);					
				}
				_aiComponent->getParent().getComponent<PhysicsComponent>()->setVelocity(
					PhysicsComponent::ZERO_VELOCITY);
			}
		}
	} 
	else
	{
		_aiComponent->ChangeState(new AiPatrol(_aiComponent, _center, _attackRadius));
		LOG_D("Going from Attack to Patrol");
	}

}
