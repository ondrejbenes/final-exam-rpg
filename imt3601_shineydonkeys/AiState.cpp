#include "AiState.h"
#include "EntityManager.h"
#include <complex>
#include "PhysicsComponent.h"
#include "Logger.h"
#include "VectorUtilities.h"

AiState::AiState(AiComponent* component): _aiComponent(component)
{

}

AiState::~AiState()
{

}

void AiState::setVelocityTowardsPosition(const sf::Vector2f& position)
{
	auto ai = _aiComponent->getParent();
	auto pc = ai.getComponent<PhysicsComponent>();
	auto aiPos = ai.getPosition();
	auto xDiff = position.x - aiPos.x;
	auto yDiff = position.y - aiPos.y;

	
	if (abs(xDiff) < 10)
	{
		// go up or down
		if (yDiff > 0)
		{
			// go down
			pc->setVelocity(sf::Vector2f(0, 1 * PhysicsComponent::defaultVelocity.y));
		}
		else if (yDiff == 0)
		{
			// go up or down
			LOG_D("Arrived at location");
		}
		else
		{
			// go up
			pc->setVelocity(sf::Vector2f(0, -1 * PhysicsComponent::defaultVelocity.y));
		}
	} 
	else if (xDiff > 0)
	{
		// go right
		pc->setVelocity(sf::Vector2f(1 * PhysicsComponent::defaultVelocity.x, 0));
	}
	else
	{
		// go left
		pc->setVelocity(sf::Vector2f(-1 * PhysicsComponent::defaultVelocity.x, 0));
	}

}

bool AiState::isPlayerInRadius(const sf::Vector2f& center, const float radius) const
{
	auto playerPos = EntityManager::getInstance()->getLocalPlayer()->getPosition();
	auto dist = VectorUtilities::calculateDistance(center, playerPos);

	return dist <= radius;
}
