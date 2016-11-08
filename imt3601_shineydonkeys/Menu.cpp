#include "Menu.h"
#include "Logger.h"
#include "Blackboard.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Sprite.hpp>

Menu::Menu()
{

}

Menu::~Menu()
{

}

void Menu::update()
{
	handleInput();
}

void Menu::render(std::shared_ptr<sf::RenderWindow> window)
{	
	
	GamePhase::render(window);
}

void Menu::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event);
			break;
		case sf::Event::TextEntered:
			//handleTextEntered(event);
			break;
		case sf::Event::KeyPressed:
			handleKeyPressed(event);
			break;
		default:
			break;
		}
	}
}

void Menu::handleMouseReleased(const sf::Event& event)
{
	auto clickedOnElement = false;

	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
	{
		auto bounds = (*it)->getBounds();
		if (bounds.contains(event.mouseButton.x, event.mouseButton.y))
		{
			clickedOnElement = true;
			_ui.setFocusedElement(*it);
			auto callback = (*it)->getOnClick();
			if (callback != nullptr)
				(*callback)(*it, event);
		}
	}

	if (!clickedOnElement)
		_ui.setFocusedElement(nullptr);
}

void Menu::handleKeyPressed(const sf::Event& event)
{
	auto focusedElement = _ui.getFocusedElement();
	if (focusedElement == nullptr)
		return;

	auto callback = focusedElement->getOnKeyPressed();
	if (callback != nullptr)
		(*callback)(focusedElement, event);
}