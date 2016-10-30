#include "AnimationComponent.h"
#include "GraphicsComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>

AnimationComponent::AnimationComponent(Entity& parent) : 
EntityComponent(parent),
spriteSheetCell(new sf::Vector2i(0, Still))
{

}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::update()
{
	if (animationTimer.getElapsedTime() > ANIMATION_PERIOD_MS)
	{
		spriteSheetCell->x++;
		animationTimer.restart();
	}
	if (spriteSheetCell->x % 64 > 7)
		spriteSheetCell->x = 0;

	auto graphicsComponent = parent.getComponent<GraphicsComponent>();
	if (graphicsComponent == nullptr) return;
	auto& sprite = graphicsComponent->getSprite();
	// TODO remove magic constants
	sprite.setTextureRect(sf::IntRect(spriteSheetCell->x % 8 * 240, spriteSheetCell->y % 8 * 240, 240, 240));
	
	auto speed = parent.getComponent<PhysicsComponent>()->getVelocity();
	if (spriteSheetCell->y == Jump || spriteSheetCell->y == Still)
	{
		return;
		//sprite.setTexture(playerTexture2);
	}

	sprite.setPosition(parent.getPosition());
}

void AnimationComponent::animate(MovementDirection directtion)
{
	if (spriteSheetCell->y != directtion)
		spriteSheetCell->x = 0;
	spriteSheetCell->y = directtion;
}

sf::Time AnimationComponent::ANIMATION_PERIOD_MS = sf::milliseconds(100);