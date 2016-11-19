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
	_parentAsChar = dynamic_cast<Character*>(&_aiComponent->getParent());
}

AiAttack::~AiAttack()
{

} 

void AiAttack::update()
{
	auto player = EntityManager::getInstance()->getLocalPlayer();
	if (player->getStats()->current_hitpoints == 0)
		return;
	auto playerPos = player->getPosition();
	auto aiPos = _aiComponent->getParent().getPosition();

	if (isPlayerInRadius(_center, _attackRadius))
	{
		if (VectorUtilities::calculateDistance(playerPos, aiPos) > Weapon::WEAPON_RANGE)
		{
			setVelocityTowardsPosition(playerPos);
			if (_parentCombatComp->isInCombat())
			{
				_parentCombatComp->endCombat();
				if (_otherCombatComp != nullptr && _otherCombatComp->getOther() == _parentAsChar)
					_otherCombatComp->endCombat();

			}
		}
		else
		{
			if (!_parentCombatComp->isInCombat())
			{
				_parentCombatComp->startCombat(player);

				if (_otherCombatComp == nullptr)
					_otherCombatComp = player->getComponent<CombatComponent>();

				if (!_otherCombatComp->isInCombat())
					_otherCombatComp->startCombat(_parentAsChar);		

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
