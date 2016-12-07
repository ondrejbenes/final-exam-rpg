#include "AiAttack.h"
#include "AiComponent.h"
#include "PhysicsComponent.h"
#include "AiPatrol.h"
#include "EntityManager.h"
#include "VectorUtilities.h"
#include "Logger.h"
#include "CombatComponent.h"
#include "EntityFactory.h"
#include "Blackboard.h"

AiAttack::AiAttack(AiComponent* component, sf::Vector2f center, float attackRadius) :
	AiState(component),
	_center(center),
	_attackRadius(attackRadius),
	_otherCombatComp(nullptr)
{
	_parentCombatComp = _aiComponent->getParent().getComponent<CombatComponent>();
	_parentAsChar = dynamic_cast<Character*>(&_aiComponent->getParent());
}

void AiAttack::update()
{
	auto player = EntityManager::getInstance()->getLocalPlayer();
	if (player->getStats().current_hitpoints == 0)
		return;
	auto playerPos = player->getPosition();
	auto aiPos = _aiComponent->getParent().getPosition();

	if (isPlayerInRadius(_center, _attackRadius))
	{
		if (VectorUtilities::calculateDistance(playerPos, aiPos) > Weapon::WEAPON_RANGE)
		{
			if (_moveTowardsPlayerClock.getElapsedTime().asMilliseconds() >= MOVE_TOWARDS_PLAYER_RECALC_FREQ_MS)
				_moveTowardsPlayerClock.restart();
			else
				return;

			setVelocityTowardsPosition(playerPos);
			if (_parentCombatComp->isInCombat())
			{
				_parentCombatComp->endCombat();
				auto aiId = _aiComponent->getParent().id;
				Blackboard::getInstance()->leaveCallback(
					NETWORK,
					[aiId](Module* target)
				{
					PacketFactory factory;
					auto network = dynamic_cast<Network*>(target);

					auto startCombatPacket = factory.createEndCombat(aiId);
					network->broadcast(startCombatPacket);
				}
				);
				if (_otherCombatComp != nullptr && _otherCombatComp->getOther() == _parentAsChar)
				{
					_otherCombatComp->endCombat();

					auto otherId = _otherCombatComp->getParent().id;
					Blackboard::getInstance()->leaveCallback(
						NETWORK,
						[otherId](Module* target)
					{
						PacketFactory factory;
						auto network = dynamic_cast<Network*>(target);

						auto startCombatPacket = factory.createEndCombat(otherId);
						network->broadcast(startCombatPacket);
					}
					);
				}
			}
		}
		else
		{
			if (!_parentCombatComp->isInCombat())
			{
				_parentCombatComp->startCombat(player);

				auto aiId = _aiComponent->getParent().id;
				auto playerId = player->id;

				Blackboard::getInstance()->leaveCallback(
					NETWORK,
					[aiId, playerId](Module* target)
				{
					PacketFactory factory;
					auto network = dynamic_cast<Network*>(target);

					auto startCombatPacket = factory.createEnterCombat(aiId, playerId);
					network->broadcast(startCombatPacket);
				}
				);

				if (_otherCombatComp == nullptr)
					_otherCombatComp = player->getComponent<CombatComponent>();

				if (!_otherCombatComp->isInCombat())
				{
					_otherCombatComp->startCombat(_parentAsChar);

					Blackboard::getInstance()->leaveCallback(
						NETWORK,
						[aiId, playerId](Module* target)
					{
						PacketFactory factory;
						auto network = dynamic_cast<Network*>(target);

						auto startCombatPacket = factory.createEnterCombat(playerId, aiId);
						network->broadcast(startCombatPacket);
					}
					);
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

float AiAttack::MOVE_TOWARDS_PLAYER_RECALC_FREQ_MS = 500;