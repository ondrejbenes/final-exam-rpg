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