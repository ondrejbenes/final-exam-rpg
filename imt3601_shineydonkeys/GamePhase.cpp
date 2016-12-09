#include "GamePhase.h"
#include <memory>

UI& GamePhase::getUi()
{
	return _ui;
}

void GamePhase::render(std::shared_ptr<sf::RenderWindow> window)
{
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
	{
		(*it)->draw(window);
	}
}

void GamePhase::handleGuiEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::MouseButtonReleased:
		handleMouseReleased(event);
		break;
	case sf::Event::TextEntered:
		handleTextEntered(event);
		break;
	case sf::Event::KeyPressed:
		handleKeyPressed(event);
		break;
	default:
		break;
	}
}

void GamePhase::handleMouseReleased(const sf::Event& event)
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
				(*callback)(it->get(), event);
		}
	}

	if (!clickedOnElement)
		_ui.setFocusedElement(nullptr);
}


void GamePhase::handleTextEntered(const sf::Event& event)
{
	auto focusedElement = _ui.getFocusedElement();
	if (focusedElement == nullptr)
		return;

	auto callback = focusedElement->getOnTextEntered();
	if (callback != nullptr)
		(*callback)(focusedElement.get(), event);
}

void GamePhase::handleKeyPressed(const sf::Event& event)
{
	auto focusedElement = _ui.getFocusedElement();
	if (focusedElement == nullptr)
		return;

	auto callback = focusedElement->getOnKeyPressed();
	if (callback != nullptr)
		(*callback)(focusedElement.get(), event);
}