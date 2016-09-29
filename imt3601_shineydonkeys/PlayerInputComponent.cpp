#include "PlayerInputComponent.h"
#include <SFML/Window/Joystick.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/System.hpp>

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent), 
source(0, Still)
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) <-50)
	{
		source.x *= source.y == sf::Keyboard::Up;//zero if new direction
		source.y = Up;

	}

	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Joystick::getAxisPosition(0, sf::Joystick::Y) >50)
	{
		source.x *= (source.y == Down);//zero if new direction
		source.y = Down;


	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) > 50)
	{
		source.x *= (source.y == Right);
		source.y = Right;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Joystick::getAxisPosition(0, sf::Joystick::X) <-50)
	{
		source.x *= (source.y == Left);
		source.y = Left;
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) || sf::Joystick::isButtonPressed(0, 2))
	{
		source.x *= (source.y == Jump);
		source.y = Jump;
	}
}