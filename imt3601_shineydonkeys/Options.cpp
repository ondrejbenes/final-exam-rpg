#include "Options.h"
#include "Blackboard.h"

#include <SFML/Graphics/Text.hpp>

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

void Options::render(std::shared_ptr<sf::RenderWindow> window)
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

void Options::handleMouseReleased(const sf::Event& event)
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
			if(callback != nullptr)
				(*callback)(*it, event);
		}
	}

	if(!clickedOnElement)
		_ui.setFocusedElement(nullptr);
}

void Options::handleKeyPressed(const sf::Event& event)
{
	auto focusedElement = _ui.getFocusedElement();
	if(focusedElement == nullptr)
		return;

	auto callback = focusedElement->getOnKeyPressed();
	if (callback != nullptr)
		(*callback)(focusedElement, event);
}
