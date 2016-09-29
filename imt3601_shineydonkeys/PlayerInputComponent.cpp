#include "PlayerInputComponent.h"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>
#include "GraphicsComponent.h"

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent),
spriteSheetCell(new sf::Vector2i(0, Still))
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <-50)
	{
		spriteSheetCell->x *= spriteSheetCell->y == sf::Keyboard::Up;//zero if new direction
		spriteSheetCell->y = Up;

	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >50)
	{
		spriteSheetCell->x *= (spriteSheetCell->y == Down);//zero if new direction
		spriteSheetCell->y = Down;


	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50)
	{
		spriteSheetCell->x *= (spriteSheetCell->y == Right);
		spriteSheetCell->y = Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) <-50)
	{
		spriteSheetCell->x *= (spriteSheetCell->y == Left);
		spriteSheetCell->y = Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 2))
	{
		spriteSheetCell->x *= (spriteSheetCell->y == Jump);
		spriteSheetCell->y = Jump;
	}

	if(animationTimer.getElapsedTime() > ANIMATION_PERIOD_MS)
	{
		spriteSheetCell->x++;
		animationTimer.restart();
	}
	if (spriteSheetCell->x % 64 > 7)
		spriteSheetCell->x == 0;
	//source->x %= 64;

	auto graphicsComponent = parent.getComponent<GraphicsComponent>();
	if (graphicsComponent == nullptr) return;

	sf::Sprite& sprite = graphicsComponent->getSprite();
	
	sprite.setTextureRect(sf::IntRect(spriteSheetCell->x % 8 * 240, spriteSheetCell->y % 8 * 240, 240, 240));

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

sf::Time PlayerInputComponent::ANIMATION_PERIOD_MS = sf::milliseconds(150);