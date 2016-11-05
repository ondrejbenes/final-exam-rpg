#include "AiPatrol.h"
#include "AiAttack.h"
#include "AiState.h"
#include "PhysicsComponent.h"
#include "EntityManager.h"
#include "VectorUtilities.h"
#include "Logger.h"

AiPatrol::AiPatrol(AiComponent* component, sf::Vector2f center, float patrolRadius) : 
AiState(component), 
_center(center),
_patrolRadius(patrolRadius)
{
	_movePoint = getNextMovePoint();
}

AiPatrol::~AiPatrol()
{

}

void AiPatrol::update()
{
	auto currentLocation = _aiComponent->getParent().getPosition();
	if(VectorUtilities::calculateDistance(currentLocation, _movePoint) < 10)
	{
		// TODO AI needs to wait a couple of seconds after getting to the move point
		_movePoint = getNextMovePoint();
	} else
	{
		// TODO don't call this every frame
		setVelocityTowardsPosition(_movePoint);
	}

	// TODO Different aggresion -> modify the _patrolRaidus
	// TODO move center, _patrolRadius etc to the Character class
	if (isPlayerInRadius(_center, _patrolRadius))
	{
		LOG_D("Going from Patrol to Attack ");
		_aiComponent->ChangeState(new AiAttack(_aiComponent, _center, _patrolRadius));
	}

	//auto playerPos = EntityManager::getInstance()->getLocalPlayer()->getPosition();
	//auto aiPos = _aiComponent->getParent().getPosition();

	//if (isPlayerInRadius(sf::Vector2f(4120, 2310), 500))
	//{
	//	if (VectorUtilities::calculateDistance(playerPos, aiPos) > 100)
	//		setVelocityTowardsPosition(playerPos);
	//	else
	//		_aiComponent->getParent().getComponent<PhysicsComponent>()->setVelocity(sf::Vector2f(0, 0));
	//} 
	//else
	//{
	//	auto pc = _aiComponent->getParent().getComponent<PhysicsComponent>();

	//	if (framesInOneDirection > 60 * 1) // 60 FPS, 1 secs
	//	{
	//		int randVelocities[] = { -PhysicsComponent::defaultVelocity.x, 0, PhysicsComponent::defaultVelocity.x };
	//		sf::Vector2f velocity;
	//		velocity.x = randVelocities[rand() % 3];
	//		velocity.y = randVelocities[rand() % 3];
	//		if (rand() % 2)
	//			velocity.x = 0;
	//		else
	//			velocity.y = 0;
	//		pc->setVelocity(velocity);
	//		framesInOneDirection = 0;
	//	}

	//	framesInOneDirection++;
	//	if (test > 300)
	//	{
	//		_aiComponent->ChangeState(new AiAttack(_aiComponent));
	//		test = 0;
	//	}
	//	else test++;
	//}
}

sf::Vector2f AiPatrol::getNextMovePoint() const
{
	auto minX = _center.x - _patrolRadius;
	auto minY = _center.y - _patrolRadius;

	auto newX = minX + rand() % int(2 * _patrolRadius);
	auto newY = minY + rand() % int(2 * _patrolRadius);

	return sf::Vector2f(newX, newY);
}
