#include "PlayerInputComponent.h"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>
#include "GraphicsComponent.h"

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent)
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	auto graphicsComponent = parent.getComponent<GraphicsComponent>();
	if(graphicsComponent == nullptr) return;
	sf::Vector2i* source = graphicsComponent->spriteSheetCell;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <-50)
	{
		source->x *= source->y == sf::Keyboard::Up;//zero if new direction
		source->y = Up;

	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >50)
	{
		source->x *= (source->y == Down);//zero if new direction
		source->y = Down;


	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50)
	{
		source->x *= (source->y == Right);
		source->y = Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) <-50)
	{
		source->x *= (source->y == Left);
		source->y = Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 2))
	{
		source->x *= (source->y == Jump);
		source->y = Jump;
	}

	if(animationTimer.getElapsedTime() > ANIMATION_PERIOD_MS)
	{
		source->x++;
		animationTimer.restart();
	}
	if (source->x % 64 > 7)
		source->x == 0;
	//source->x %= 64;
}

sf::Time PlayerInputComponent::ANIMATION_PERIOD_MS = sf::milliseconds(150);