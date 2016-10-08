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
		auto console = Console::getInstance();
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Tilde)
			{
				console->setVisible(!console->isVisible());
				input = "";
			}

			if (event.key.code == sf::Keyboard::Return)
			{
				if(console->isVisible())
					console->handleInput();
				input = "";
			}
			break;
		case sf::Event::TextEntered:
			if (event.text.unicode == ';') break;
			if (event.text.unicode == '\r') break;
			input += event.text.unicode;
			if (console->isVisible())
				console->setInput(input);
			break;
		default:
			LOG_D("Unknown event");
			break;
		}
	}

	// handleConsole();
	handleMovement();
	handleLoadAndSave();
}

void PlayerInputComponent::handleConsole()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tilde))
	{
		auto console = Console::getInstance();
		console->setVisible(!console->isVisible());
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


void PlayerInputComponent::handleLoadAndSave()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F5))
	{
		LOG_I("Saving game");
		auto blackboard = Blackboard::getInstance();
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