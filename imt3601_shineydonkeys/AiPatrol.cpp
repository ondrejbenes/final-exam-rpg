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

void AiPatrol::update()
{
	if (_timeSinceLastUpdate.getElapsedTime().asSeconds() < UPDATE_FREQUENCY)
		return;

	_timeSinceLastUpdate.restart();
	
	auto currentLocation = _aiComponent->getParent().getPosition();
	if(VectorUtilities::calculateDistance(currentLocation, _movePoint) < CHANGE_MOVE_POINT_DISTANCE)
		_movePoint = getNextMovePoint();
	else
		setVelocityTowardsPosition(_movePoint);

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
unsigned AiPatrol::CHANGE_MOVE_POINT_DISTANCE = 10;