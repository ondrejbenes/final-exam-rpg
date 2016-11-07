#include "StartMultiPlayerGame.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include "Options.h"
#include "Blackboard.h"
#include <memory>


StartMultiPlayerGame::StartMultiPlayerGame()
{

}


StartMultiPlayerGame::~StartMultiPlayerGame()
{

}

void StartMultiPlayerGame::update()
{
	handleInput();
}

void StartMultiPlayerGame::render(std::shared_ptr<sf::RenderWindow> window)
{

	GamePhase::render(window);
}

// TODO duplicity with Menu.cpp
void StartMultiPlayerGame::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event);
		case sf::Event::KeyPressed:
			break;
		default:
			break;
		}
	}
}

void StartMultiPlayerGame::handleMouseReleased(const sf::Event& event)
{
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
	{
		auto bounds = (*it)->getBounds();
		if (bounds.contains(event.mouseButton.x, event.mouseButton.y))
		{
			auto callback = (*it)->getOnClick();
			(*callback)();
		}
	}
}