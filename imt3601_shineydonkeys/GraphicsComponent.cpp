#include "GraphicsComponent.h"
#include "Logger.h"
#include "PlayerInputComponent.h"

GraphicsComponent::GraphicsComponent(Entity& parent) : EntityComponent(parent),
spriteSheetCell(new sf::Vector2i(0, Still))
{
	if (!playerTexture.loadFromFile("Resources/Images/noaxereally.png"))
		LOG_E("Error: could not load player image");
	spriteSheet.setTexture(playerTexture);
}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::draw(sf::RenderWindow* window) const
{
	window->draw(spriteSheet);
}

void GraphicsComponent::update()
{
	spriteSheet.setTextureRect(sf::IntRect(spriteSheetCell->x % 8 * 240, spriteSheetCell->y % 8 * 240, 240, 240));

	auto speed = 10.0f;

	if (spriteSheetCell->y == Left)
	{
		spriteSheet.move(-1 * speed, 0);

	}

	if (spriteSheetCell->y == Right)
	{

		spriteSheet.move(1 * speed, 0);

	}

	if (spriteSheetCell->y == Down)
	{
		spriteSheet.move(0, 1 * speed);

	}


	if (spriteSheetCell->y == Up)
	{
		spriteSheet.move(0, -1 * speed);

	}
}
