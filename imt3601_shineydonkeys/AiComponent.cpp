#include "AiComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

AiComponent::AiComponent(Entity& parent) : 
EntityComponent(parent)
{

}

AiComponent::~AiComponent()
{

}

void AiComponent::update()
{
	auto pc = parent.getComponent<PhysicsComponent>();

	if(framesInOneDirection > 60 * 1) // 60 FPS, 1 secs
	{
		int randVelocities[] = { -10, 0, 10 };
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

	pc->move();
	framesInOneDirection++;	
}