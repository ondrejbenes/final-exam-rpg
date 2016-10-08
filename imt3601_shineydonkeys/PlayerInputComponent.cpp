#include "PlayerInputComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include "GraphicsComponent.h"
#include "AnimationComponent.h"
#include "PhysicsComponent.h"
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
	auto pc = parent.getComponent<PhysicsComponent>();
	auto defaultVelocity = PhysicsComponent::defaultVelocity;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		pc->setVelocity(sf::Vector2f(0, -1 * defaultVelocity.y));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		pc->setVelocity(sf::Vector2f(0, defaultVelocity.y));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		pc->setVelocity(sf::Vector2f(defaultVelocity.x, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		pc->setVelocity(sf::Vector2f(-1 * defaultVelocity.x, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		// TODO remove?
		//pc->move(Jump);
	} else
	{
		pc->setVelocity(sf::Vector2f(0, 0));
	}
	pc->move();
}
