#include "AiComponent.h"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "GraphicsComponent.h"


AiComponent::AiComponent(Entity& parent) : 
EntityComponent(parent),
spriteSheetCell(new sf::Vector2i(0, Still))
{

}



AiComponent::~AiComponent()
{

}





void AiComponent::update()
{
	if (animationTimer.getElapsedTime() > ANIMATION_PERIOD_MS)
	{
		spriteSheetCell->x++;
		animationTimer.restart();
	}
	if (spriteSheetCell->x % 64 > 7)
		spriteSheetCell->x == 0;

	auto graphicsComponent = parent.getComponent<GraphicsComponent>();
	if (graphicsComponent == nullptr) return;

	sf::Sprite& sprite = graphicsComponent->getSprite();

	sprite.setTextureRect(sf::IntRect(spriteSheetCell->x % 8 * 240, spriteSheetCell->y % 8 * 240, 240, 240));

	if(framesInOneDirection > 60 * 1) // 60 FPS, 1 secs
	{
		int dirs[] = { Left, Right, Down, Up };
		spriteSheetCell->y = dirs[rand() % 4];
		framesInOneDirection = 0;
	}
	framesInOneDirection++;

	auto speed = 10.0f;
	if (spriteSheetCell->y == Jump)
	{
		sprite.move(0 * speed, 0);
		//sprite.setTexture(playerTexture2);
	}

	if (spriteSheetCell->y == Left)
	{
		sprite.move(-1 * speed, 0);
	}

	if (spriteSheetCell->y == Right)
	{
		sprite.move(1 * speed, 0);
	}

	if (spriteSheetCell->y == Down)
	{
		sprite.move(0, 1 * speed);
	}


	if (spriteSheetCell->y == Up)
	{
		sprite.move(0, -1 * speed);
	}
}

sf::Time AiComponent::ANIMATION_PERIOD_MS = sf::milliseconds(150);