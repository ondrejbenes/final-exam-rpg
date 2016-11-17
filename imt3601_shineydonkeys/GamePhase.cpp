#include "GamePhase.h"
#include <memory>

UI& GamePhase::getUi()
{
	return _ui;
}

GamePhase::GamePhase(){

}

GamePhase::~GamePhase(){

}

void GamePhase::render(std::shared_ptr<sf::RenderWindow> window)
{
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
	{
		(*it)->update();
		(*it)->draw(window);
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
				(*callback)(*it, event);
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
		(*callback)(focusedElement, event);
}
