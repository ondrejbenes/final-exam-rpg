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
	if (_timeSinceLastUpdate.getElapsedTime().asSeconds() >= UPDATE_FREQUENCY)
		_timeSinceLastUpdate.restart();
	else
		return;

	auto currentLocation = _aiComponent->getParent().getPosition();
	if(VectorUtilities::calculateDistance(currentLocation, _movePoint) < 10)
	{
		// TODO AI needs to wait a couple of seconds after getting to the move point
		_movePoint = getNextMovePoint();
	} else
	{
		setVelocityTowardsPosition(_movePoint);
	}

	// TODO Different aggresion -> modify the _patrolRaidus
	// TODO move center, _patrolRadius etc to the Character class
	if (isPlayerInRadius(_center, _patrolRadius))
	{
		LOG_D("Going from Patrol to Attack ");
		_aiComponent->ChangeState(new AiAttack(_aiComponent, _center, _patrolRadius));
	}
}

sf::Vector2f AiPatrol::getNextMovePoint() const
{
	auto minX = _center.x - _patrolRadius;
	auto minY = _center.y - _patrolRadius;

	auto newX = minX + rand() % int(2 * _patrolRadius);
	auto newY = minY + rand() % int(2 * _patrolRadius);

	return sf::Vector2f(newX, newY);
}

float AiPatrol::UPDATE_FREQUENCY = 1;
