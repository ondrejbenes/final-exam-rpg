#include "AI_Patrol.h"
#include "PhysicsComponent.h"


AiPatrol::AiPatrol(AiComponent* component): AiState(component)    // RENAME ALL OF THEM LIKE THIS
{
	}


AiPatrol::~AiPatrol()
{
}

void AiPatrol::update()
{
	auto pc = _AiComponent->getParent().getComponent<PhysicsComponent>();

	if (framesInOneDirection > 60 * 1) // 60 FPS, 1 secs
	{
		int randVelocities[] = { -PhysicsComponent::defaultVelocity.x, 0, PhysicsComponent::defaultVelocity.x };
		sf::Vector2f velocity;
		velocity.x = randVelocities[rand() % 3];
		velocity.y = randVelocities[rand() % 3];
		if (rand() % 2)
			velocity.x = 0;
		else
			velocity.y = 0;
		pc->setVelocity(velocity);
		framesInOneDirection = 0;
	}

	framesInOneDirection++;
}
