#include "PlayerInputComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include "GraphicsComponent.h"
#include "AnimationComponent.h"
#include "Logger.h"
#include "Blackboard.h"
#include "Module.h"
#include "Game.h"

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent)
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	handleMovement();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		LOG_I("Saving game");
		auto blackboard = Blackboard::getInstance();
		//blackboard->leaveMessage(GAME, new SaveGameCommand);
		blackboard->leaveCallback(GAME, [](Module* target)
		{
			dynamic_cast<Game*>(target)->saveGame();
		});
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::F6))
	{
		LOG_I("Loading game");
	}
}

void PlayerInputComponent::handleMovement()
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
	} else
	{
		ac->animate(Still);
	}
}
