#include "PlayerInputComponent.h"
#include <SFML/Window/Keyboard.hpp>
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Logger.h"
#include "Blackboard.h"
#include "Module.h"
#include "Game.h"
#include "Console.h"

PlayerInputComponent::PlayerInputComponent(Entity& parent) : 
EntityComponent(parent)
{

}

PlayerInputComponent::~PlayerInputComponent()
{

}

void PlayerInputComponent::update()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event; 
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::F5)
			{
				LOG_I("Saving game");
				blackboard->leaveCallback(GAME, [](Module* target)
				{
					dynamic_cast<Game*>(target)->quickSave();
				});
			}

			if (event.key.code == sf::Keyboard::F6)
			{
				LOG_I("Loading game");
				blackboard->leaveCallback(GAME, [](Module* target)
				{
					dynamic_cast<Game*>(target)->quickLoad();
				});
			}
			break;
		default:
			LOG_D("Unknown event");
			break;
		}
	}

	handleMovement();
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