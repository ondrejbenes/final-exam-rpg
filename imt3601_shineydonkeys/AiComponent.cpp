#include "AiComponent.h"
#include "GraphicsComponent.h"
#include "AnimationComponent.h"

AiComponent::AiComponent(Entity& parent) : 
EntityComponent(parent)
{

}

AiComponent::~AiComponent()
{

}

void AiComponent::update()
{
	if(framesInOneDirection > 60 * 1) // 60 FPS, 1 secs
	{
		MovementDirection dirs[] = { Left, Right, Down, Up, Still, Jump };
		auto ac = parent.getComponent<AnimationComponent>();
		ac->animate(dirs[rand() % 6]);
		framesInOneDirection = 0;
	}
	framesInOneDirection++;	
}