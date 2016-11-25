#include "AiState.h"
#include "EntityManager.h"
#include <complex>
#include "PhysicsComponent.h"
#include "Logger.h"
#include "VectorUtilities.h"
#include "Network.h"
#include "Blackboard.h"

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

	sf::Vector2f newVelocity;
	if (abs(xDiff) < 10)
	{
		// go up or down
		if (yDiff > 0)
		{
			// go down
			newVelocity = sf::Vector2f(0, 1 * PhysicsComponent::DEFAULT_NPC_VELOCITY.y);
		}
		else
		{
			// go up
			newVelocity = sf::Vector2f(0, -1 * PhysicsComponent::DEFAULT_NPC_VELOCITY.y);
		}
	} 
	else if (xDiff > 0)
	{
		// go right
		newVelocity = sf::Vector2f(1 * PhysicsComponent::DEFAULT_NPC_VELOCITY.x, 0);
	}
	else
	{
		// go left
		newVelocity = sf::Vector2f(-1 * PhysicsComponent::DEFAULT_NPC_VELOCITY.x, 0);
	}

	if(newVelocity != pc->getVelocity())
		pc->setVelocity(newVelocity);
}

bool AiState::isPlayerInRadius(const sf::Vector2f& center, const float radius) const
{
	auto playerPos = EntityManager::getInstance()->getLocalPlayer()->getPosition();
	auto dist = VectorUtilities::calculateDistance(center, playerPos);

	return dist <= radius;
}
