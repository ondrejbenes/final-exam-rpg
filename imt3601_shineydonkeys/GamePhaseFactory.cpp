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
	auto configFile = L"./Config/main_menu.ini";

	auto singlePlayer = createMenuUiElement(
		configFile, 
		L"singleplayer", 
		[&]() { GamePhaseManager::getInstance()->pushPhase(createMainGame()); });

	auto multiPlayer = createMenuUiElement(
		configFile,
		L"multiplayer",
		[&]() { GamePhaseManager::getInstance()->pushPhase(createStartMultiPlayerGame()); });

	auto options = createMenuUiElement(
		configFile,
		L"options",
		[&]() { GamePhaseManager::getInstance()->pushPhase(createOptions()); });

	auto exit = createMenuUiElement(
		configFile,
		L"exit",
		[]()
		{
			Blackboard::getInstance()->leaveCallback(GAME, 
			[](Module* target)
			{
				dynamic_cast<Game*>(target)->stop();
			});
		});

	auto menu = new Menu;
	menu->_ui.addElement(singlePlayer);
	menu->_ui.addElement(multiPlayer);
	menu->_ui.addElement(options);
	menu->_ui.addElement(exit);

	return menu;
}

StartMultiPlayerGame* GamePhaseFactory::createStartMultiPlayerGame()
{
	auto back = createMenuUiElement(
		L"./Config/multiplayer.ini",
		L"back",
		[]() { GamePhaseManager::getInstance()->popPhase(); });

	auto startMultiPlayerGame = new StartMultiPlayerGame;
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Options* GamePhaseFactory::createOptions()
{
	auto back = createMenuUiElement(
		L"./Config/options.ini",
		L"back",
		[]() { GamePhaseManager::getInstance()->popPhase(); });

	auto options = new Options;
	options->_ui.addElement(back);

	return options;
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
