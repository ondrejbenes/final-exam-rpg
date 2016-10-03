#include "PlayerInputComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include "GraphicsComponent.h"
#include "AnimationComponent.h"

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent)
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	auto ac = parent.getComponent<AnimationComponent>();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		ac->animate(Up);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		ac->animate(Down);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		ac->animate(Right);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		ac->animate(Left);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		ac->animate(Jump);
	}
}