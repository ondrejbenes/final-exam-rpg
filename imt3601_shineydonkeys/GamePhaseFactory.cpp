#include "GamePhaseFactory.h"
#include "ResourceLoader.h"
#include "Logger.h"
#include "GamePhaseManager.h"
#include "Blackboard.h"
#include "Module.h"
#include "Game.h"

#include <SFML/Graphics/Text.hpp>
#include "ConfigIO.h"

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
	auto singlePlayer = createMenuUiElement(
		L"./Config/main_menu.ini", 
		L"singleplayer", 
		[]() { GamePhaseManager::getInstance()->pushPhase(new MainGame); });

	auto exit = createMenuUiElement(
		L"./Config/main_menu.ini",
		L"exit",
		[]()
		{
			Blackboard::getInstance()->leaveCallback(GAME, [](Module* target)
			{
				dynamic_cast<Game*>(target)->stop();
			});
		});

	auto menu = new Menu;
	menu->_ui.addElement(singlePlayer);
	menu->_ui.addElement(exit);

	return menu;
}

UiElement* GamePhaseFactory::createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, std::function<void()> onClick)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto singlePlayerText = new sf::Text(text, font, fontSize);
	singlePlayerText->setPosition(x, y);
	singlePlayerText->setFillColor(sf::Color::White);

	auto uiElement = new UiElement(singlePlayerText);
	uiElement->setOnClick(new std::function<void()>(onClick));

	return uiElement;
}