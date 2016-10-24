#include "GamePhase.h"

UI& GamePhase::getUi()
{
	return _ui;
}

GamePhase::GamePhase(){

}

GamePhase::~GamePhase(){

}

void GamePhase::render(sf::RenderWindow* window)
{
	auto uiElements = _ui.getElements();
	for (auto it = uiElements.begin(); it != uiElements.end(); ++it)
		window->draw(*(*it)->getGraphics());
}