#include "AI_Follow.h"
#include "AiComponent.h"
#include "PhysicsComponent.h"
#include "AI_Patrol.h"


AiFollow::AiFollow(AiComponent* component) : AiState(component)
{
}


AiFollow::~AiFollow()
{
} 

void AiFollow::update()
{
	auto pc = _AiComponent->getParent().getComponent<PhysicsComponent>();
	//_AiComponent->ChangeState(new AiFollow(_AiComponent));
	//pc->move();

	{
		//int randVelocities[] = { -PhysicsComponent::defaultVelocity.x, 0, PhysicsComponent::defaultVelocity.x };
		sf::Vector2f velocity;
		//velocity.x = randVelocities[rand() % 3];
		//velocity.y = randVelocities[rand() % 3];
		//if (rand() % 2)
		//	velocity.x = 0;
		//else
		//	velocity.y = 0;
		velocity.x =  100;
		velocity.y = -100;

		pc->setVelocity(velocity);
		framesInOneDirection = 0;
	}


	// testing how velocity works
	if (test > 100)
	{
		_AiComponent->ChangeState(new AiPatrol(_AiComponent));
		test = 0;
	}
	else test++;

	framesInOneDirection++;


}
