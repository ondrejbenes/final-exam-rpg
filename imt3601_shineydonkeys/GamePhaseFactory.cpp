#include "GamePhaseFactory.h"
#include "ResourceLoader.h"

#include <SFML/Graphics/Text.hpp>
#include "Logger.h"
#include "GamePhaseManager.h"


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
	auto singlePlayerText = new sf::Text("Singleplayer", ResourceLoader::getInstance()->getMenuFont(), 20);
	singlePlayerText->setPosition(50, 50);
	singlePlayerText->setFillColor(sf::Color::Blue);

	auto singlePlayer = new UiElement(singlePlayerText);
	auto onClickSinglePlayer = new std::function<void()>;
	*onClickSinglePlayer = []() { GamePhaseManager::getInstance()->pushPhase(new MainGame); };
	singlePlayer->setOnClick(onClickSinglePlayer);

	auto exitText = new sf::Text("Exit", ResourceLoader::getInstance()->getMenuFont(), 20);
	exitText->setPosition(50, 75);
	exitText->setFillColor(sf::Color::Blue);

	auto exit = new UiElement(exitText);
	auto onClickExit = new std::function<void()>;
	*onClickExit = []() { LOG_D("EXITING"); };
	exit->setOnClick(onClickExit);

	auto menu = new Menu;
	menu->_ui.addElement(singlePlayer);
	menu->_ui.addElement(exit);

	return menu;
}