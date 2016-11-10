#include "GamePhaseFactory.h"
#include "ResourceLoader.h"
#include "Logger.h"
#include "GamePhaseManager.h"
#include "Blackboard.h"
#include "Module.h"
#include "Game.h"
#include "ConfigIO.h"
#include "Button.h"
#include "StringUtilities.h"
#include "Network.h"
#include "PacketFactory.h"

#include <sstream>

#include <SFML/Network/IpAddress.hpp>

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
		{
			Blackboard::getInstance()->leaveCallback(
				NETWORK,
				[](Module* target)
				{
					dynamic_cast<Network*>(target)->clear();
				});
			GamePhaseManager::getInstance()->pushPhase(createMainGame());
		});

	auto startMP = createMenuUiElement(
		configFile,
		L"startMP",
		[&](UiElement* source, const sf::Event& event)
		{
			Blackboard::getInstance()->leaveCallback(
				NETWORK,
				[](Module* target)
			{
				auto networkModule = dynamic_cast<Network*>(target);
				networkModule->initAsServer();
			});
			GamePhaseManager::getInstance()->pushPhase(createStartMultiPlayerGame());
		});

	auto joinMP = createMenuUiElement(
		configFile,
		L"joinMP",
		[&](UiElement* source, const sf::Event& event)
		{ GamePhaseManager::getInstance()->pushPhase(createJoinMultiPlayerGame()); });

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
	menu->_ui.addElement(startMP);
	menu->_ui.addElement(joinMP);
	menu->_ui.addElement(options);
	menu->_ui.addElement(exit);

	return menu;
}

Menu* GamePhaseFactory::createStartMultiPlayerGame()
{
	auto configFile = L"./Config/startMP.ini";

	auto ip = createMenuUiElement(
		configFile,
		L"ip",
		[](UiElement* source, const sf::Event& event)
		{ /* just a display */ });

	// TODO unnecessary to use ss?
	std::stringstream ss;
	ss << "Your IP is: " << sf::IpAddress::getLocalAddress();
	auto ipAsBtn = dynamic_cast<Button*>(ip);
	ipAsBtn->setText(ss.str());

	auto spectrator1 = createMenuUiElement(
		configFile,
		L"spectrator1",
		[](UiElement* source, const sf::Event& event)
		{ /* just a display */ });
	spectrator1->setName("spectrator1");

	auto spectrator2 = createMenuUiElement(
		configFile,
		L"spectrator2",
		[](UiElement* source, const sf::Event& event)
		{ /* just a display */ });
	spectrator2->setName("spectrator2");

	auto start = createMenuUiElement(
		configFile,
		L"start",
		[&](UiElement* source, const sf::Event& event)
		{
			Blackboard::getInstance()->leaveCallback(
				NETWORK,
				[](Module* target)
				{
					PacketFactory factory;
					auto packet = factory.createGameStarted();
					std::string msg;
					packet >> msg;
					LOG_D(msg);

					dynamic_cast<Network*>(target)->broadcast(packet);
				});
			GamePhaseManager::getInstance()->pushPhase(createMainGame());
		});

	auto back = createMenuUiElement(
		configFile,
		L"back",
		[](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->popPhase(); });

	auto startMultiPlayerGame = new Menu;
	startMultiPlayerGame->_ui.addElement(ip);
	startMultiPlayerGame->_ui.addElement(spectrator1);
	startMultiPlayerGame->_ui.addElement(spectrator2);
	startMultiPlayerGame->_ui.addElement(start);
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Menu* GamePhaseFactory::createJoinMultiPlayerGame()
{
	auto configFile = L"./Config/joinMP.ini";

	auto lambda = [](UiElement* source, const sf::Event& event)
	{
		auto sourceAsBtn = dynamic_cast<Button*>(source);
		
		auto str = sourceAsBtn->getText();

		if (event.text.unicode == 8) // backspace
			str = str.substr(0, str.length() - 1);
		else
			str += event.text.unicode;

		sourceAsBtn->setText(str);
	};
	auto ip = createMenuUiElement(
		configFile,
		L"ip", 
		[](UiElement* source, const sf::Event& event){/* only gain focus */});

	ip->setOnTextEntered(new uiCallback(lambda));
	ip->setName("serverIp");

	auto connect = createMenuUiElement(
		configFile,
		L"connect",
		[&](UiElement* source, const sf::Event& event)
		{
			Blackboard::getInstance()->leaveCallback(
				NETWORK,
				[](Module* target)
				{
					auto networkModule = dynamic_cast<Network*>(target);

					auto serverIpField = GamePhaseManager::getInstance()->getCurrentPhase()->getUi().getElementByName("serverIp");
					auto serverIpFieldStr = dynamic_cast<Button*>(serverIpField)->getText();
					std::string str{ "Server IP: " };
					auto ipAsStr = serverIpFieldStr.substr(str.length());
					LOG_D(ipAsStr);
					networkModule->setServerIp(sf::IpAddress(ipAsStr));
					networkModule->initAsClient();
				});
		});

	auto back = createMenuUiElement(
		configFile,
		L"back",
		[](UiElement* source, const sf::Event& event)
	{
		Blackboard::getInstance()->leaveCallback(
			NETWORK,
			[](Module* target)
			{
				dynamic_cast<Network*>(target)->clear();
			});
		GamePhaseManager::getInstance()->popPhase();
	});

	auto startMultiPlayerGame = new Menu;
	startMultiPlayerGame->_ui.addElement(ip);
	startMultiPlayerGame->_ui.addElement(connect);
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Menu* GamePhaseFactory::createOptions()
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
	moveLeftKey->setOnKeyPressed(new uiCallback(lambda));

	auto back = createMenuUiElement(
		L"./Config/options.ini",
		L"back",
		[](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->popPhase(); });

	auto options = new Menu;
	options->_ui.addElement(back);
	options->_ui.addElement(moveLeftKey);

	return options;
}

UiElement* GamePhaseFactory::createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, uiCallback onClick)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	UiElement* uiElement = new Button(text, font, fontSize);
	uiElement->setOnClick(new uiCallback(onClick));
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}
