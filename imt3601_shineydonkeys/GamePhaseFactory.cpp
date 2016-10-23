#include "GamePhaseFactory.h"
#include "ResourceLoader.h"

#include <SFML/Graphics/Text.hpp>


GamePhaseFactory::GamePhaseFactory()
{
}


GamePhaseFactory::~GamePhaseFactory()
{
}

MainGame* GamePhaseFactory::createMainGame()
{
	auto ret = new MainGame;



	return ret;
}

Menu* GamePhaseFactory::createMainMenu()
{
	auto startGame = new sf::Text("Start the game, sucker!", ResourceLoader::getInstance()->getMenuFont(), 20);
	startGame->setPosition(50, 50);
	startGame->setFillColor(sf::Color::Blue);

	auto menu = new Menu;
	menu->_ui.addElement(startGame);



	return menu;
}