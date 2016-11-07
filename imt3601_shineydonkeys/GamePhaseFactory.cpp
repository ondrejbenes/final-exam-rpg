#include "GamePhaseFactory.h"
#include "ResourceLoader.h"
#include "Logger.h"
#include "GamePhaseManager.h"
#include "Blackboard.h"
#include "Module.h"
#include "Game.h"

#include <SFML/Graphics/Text.hpp>
#include "ConfigIO.h"
#include "Button.h"
#include "StringUtilities.h"
#include <sstream>

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
		[&](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->pushPhase(createMainGame()); });

	auto multiPlayer = createMenuUiElement(
		configFile,
		L"multiplayer",
		[&](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->pushPhase(createStartMultiPlayerGame()); });

	auto options = createMenuUiElement(
		configFile,
		L"options",
		[&](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->pushPhase(createOptions()); });

	auto exit = createMenuUiElement(
		configFile,
		L"exit",
		[](UiElement* source, const sf::Event& event)
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
		[](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->popPhase(); });

	auto startMultiPlayerGame = new StartMultiPlayerGame;
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Options* GamePhaseFactory::createOptions()
{
	auto moveLeftKey = createMenuUiElement(
		L"./Config/options.ini",
		L"moveLeftKey",
		[](UiElement* source, const sf::Event& event) 
		{ /* only set this element as focused*/ });
	auto lambda = [](UiElement* source, const sf::Event& event)
	{
		auto sfCode = event.key.code;
		ConfigIO::writeInt(L"controls", L"left", sfCode);

		auto humanReadable = new char[20];
		StringUtilities::SFKeyToString(sfCode, humanReadable);
		std::stringstream ss;
		ss << "Move left: " << humanReadable;
		delete humanReadable;

		auto str = ss.str();

		auto sourceAsBtn = dynamic_cast<Button*>(source);
		sourceAsBtn->setText(str);

		std::wstring ws;
		ws.assign(str.begin(), str.end());

		ConfigIO::writeString(L"moveLeftKey", L"text", ws.c_str(), L"./Config/options.ini");
	};
	moveLeftKey->setOnKeyPressed(new std::function<void(UiElement* source, const sf::Event& event)>(lambda));

	auto back = createMenuUiElement(
		L"./Config/options.ini",
		L"back",
		[](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->popPhase(); });

	auto options = new Options;
	options->_ui.addElement(back);
	options->_ui.addElement(moveLeftKey);

	return options;
}

UiElement* GamePhaseFactory::createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, std::function<void(UiElement* source, const sf::Event& event)> onClick)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	UiElement* uiElement = new Button(text, font, fontSize);
	uiElement->setOnClick(new std::function<void(UiElement* source, const sf::Event& event)>(onClick));
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}
