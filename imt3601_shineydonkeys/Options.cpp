#include "Options.h"
#include "Blackboard.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

Options::Options()
{

}

Options::~Options()
{

}

void Options::update()
{

	handleInput();
}

void Options::render(sf::RenderWindow* window)
{

	GamePhase::render(window);
}

// TODO duplicity with Menu.cpp
void Options::handleInput()
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

void Options::handleMouseReleased(const sf::Event& event)
{
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
	{
		auto graphics = (*it)->getGraphics();
		sf::FloatRect bounds;
		if (typeid(*graphics) == typeid(sf::Text))
			bounds = static_cast<sf::Text*>(graphics)->getGlobalBounds();
		else if (typeid(*graphics) == typeid(sf::Sprite))
			bounds = static_cast<sf::Text*>(graphics)->getGlobalBounds();
		else
			return;

		if (bounds.contains(event.mouseButton.x, event.mouseButton.y))
		{
			auto callback = (*it)->getOnClick();
			(*callback)();
		}
	}
}