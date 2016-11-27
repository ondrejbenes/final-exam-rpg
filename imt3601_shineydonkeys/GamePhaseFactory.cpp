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
#include "TextBox.h"
#include "Audio.h"
#include "Image.h"
#include "Minimap.h"

#include <sstream>
#include <codecvt>

#include <SFML/Network/IpAddress.hpp>
#include "ChatBoard.h"
#include "Inventory.h"
#include "EntityManager.h"

MainGame* GamePhaseFactory::createMainGame()
{
	auto texture = new sf::Texture();
	if (!texture->loadFromFile("Resources/Images/minimap2.png"))
		LOG_E("Error loading minimap texture");
	auto minimap = new Minimap(texture);

	auto lambda = [](UiElement* source, const sf::Event& event)
	{
		auto chatBoard = dynamic_cast<ChatBoard*>(source);

		if (!chatBoard->isFocused())
			return;

		auto& input = chatBoard->getInput();

		if (event.text.unicode == 8) // backspace
			input = input.substr(0, input.length() - 1);
		else if (event.text.unicode == 13) // enter
		{
			auto playerName = ConfigIO::readString(L"player", L"name", L"Player");
			chatBoard->addMessage(playerName, input);
			auto msg = playerName + ": " + input;
			input = "";
			// TODO terrible encapsulation
			
			Blackboard::getInstance()->leaveCallback(NETWORK,
				[msg](Module* target)
				{
					PacketFactory factory;
					auto packet = factory.createChatMessage(msg);
					auto network = dynamic_cast<Network*>(target);
					network->broadcast(packet);
				}
			);
		}
		else
			input += event.text.unicode;

	};
	auto chatBoard = new ChatBoard();
	// TODO directly to chatboard?
	chatBoard->setOnTextEntered(new UiCallback(lambda));
	chatBoard->setName("chatBoard");

	chatBoard->addMessage("Shiny Donkey", "Hey you! Come here.");

	auto inventory = new Inventory();
	inventory->setHighlightedItem(0);

	auto inventoryLambda = [](UiElement* source, const sf::Event& event)
	{
		auto inventory = dynamic_cast<Inventory*>(source);
		auto bounds = inventory->getBounds();

		auto x = event.mouseButton.x - bounds.left;
		auto y = event.mouseButton.y - bounds.top;

		auto column = floor(x / (Inventory::width / Inventory::iconsPerRow));
		auto row = floor(y / (Inventory::height / (Inventory::maxItems / Inventory::iconsPerRow)));

		auto itemToHighlight = unsigned(row * Inventory::iconsPerRow + column);

		auto player = EntityManager::getInstance()->getLocalPlayer();
		auto playerInventory = player->getInventory();

		if (playerInventory.size() <= itemToHighlight)
			return;

		auto item = playerInventory[itemToHighlight];

		if(typeid(*item) == typeid(Weapon))
		{
			inventory->setHighlightedItem(itemToHighlight);
			player->setEquipedWeapon(std::dynamic_pointer_cast<Weapon>(item));
		}
	};
	inventory->setOnClick(new UiCallback(inventoryLambda));

	auto mainGame = new MainGame;
	mainGame->_ui.addElement(minimap);
	mainGame->_ui.addElement(chatBoard);
	mainGame->_ui.addElement(inventory);

	return mainGame;
}

Menu* GamePhaseFactory::createMainMenu()
{
	auto configFile = L"./Config/main_menu.ini";

	auto gamenamelabel = _uiElementFactory.createLabel(
		configFile,
		L"gamenamelabel");

	auto singlePlayer = _uiElementFactory.createButton(
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

	auto startMP = _uiElementFactory.createButton(
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

	auto joinMP = _uiElementFactory.createButton(
		configFile,
		L"joinMP",
		[&](UiElement* source, const sf::Event& event)
		{ GamePhaseManager::getInstance()->pushPhase(createJoinMultiPlayerGame()); });

	auto options = _uiElementFactory.createButton(
		configFile,
		L"options",
		[&](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->pushPhase(createOptions()); });

	auto exit = _uiElementFactory.createButton(
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
	menu->_ui.addElement(gamenamelabel);
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

	auto ip = _uiElementFactory.createLabel(configFile, L"ip");

	auto ipStr = "Your IP is: " + sf::IpAddress::getLocalAddress().toString();
	auto ipAsBtn = dynamic_cast<Label*>(ip);
	ipAsBtn->setText(ipStr);

	auto spectratorsHeader = _uiElementFactory.createLabel(configFile, L"spectratorsHeader");

	auto start = _uiElementFactory.createButton(
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

	auto back = _uiElementFactory.createButton(
		configFile,
		L"back",
		[](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->popPhase(); });

	auto startMultiPlayerGame = new Menu;
	startMultiPlayerGame->_ui.addElement(ip);
	startMultiPlayerGame->_ui.addElement(spectratorsHeader);
	startMultiPlayerGame->_ui.addElement(start);
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Menu* GamePhaseFactory::createJoinMultiPlayerGame()
{
	auto configFile = L"./Config/joinMP.ini";

	auto ip = _uiElementFactory.createLabel(configFile, L"serverIpLabel");

	auto serverIpTextBox = _uiElementFactory.createTextBox(configFile, L"serverIpTextBox");
	serverIpTextBox->setName("serverIp");

	auto connect = _uiElementFactory.createButton(
		configFile,
		L"connect",
		[&](UiElement* source, const sf::Event& event)
		{
			Blackboard::getInstance()->leaveCallback(
				NETWORK,
				[](Module* target)
				{
					auto networkModule = dynamic_cast<Network*>(target);
					auto textBox = GamePhaseManager::getInstance()->getCurrentPhase()->getUi().getElementByName("serverIp");
					auto ipAsStr = dynamic_cast<TextBox*>(textBox)->getText();
					LOG_I("Connecting to: " + ipAsStr);
					networkModule->setServerIp(sf::IpAddress(ipAsStr));
					networkModule->initAsClient();
				});
		});

	auto back = _uiElementFactory.createButton(
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
	startMultiPlayerGame->_ui.addElement(serverIpTextBox);	
	startMultiPlayerGame->_ui.addElement(connect);
	startMultiPlayerGame->_ui.addElement(back);

	return startMultiPlayerGame;
}

Menu* GamePhaseFactory::createOptions()
{
	auto controlsHeader = _uiElementFactory.createLabel(L"./Config/options.ini", L"controlsHeader");

	auto moveLeftKey = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"moveLeftKey",
		[](UiElement* source, const sf::Event& event) 
		{ /* only set this element as focused*/ });
	moveLeftKey->setOnKeyPressed(_uiElementFactory.createChangeKeyButton(L"left"));

	auto moveRightKey = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"moveRightKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveRightKey->setOnKeyPressed(_uiElementFactory.createChangeKeyButton(L"right"));

	auto moveUpKey = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"moveUpKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveUpKey->setOnKeyPressed(_uiElementFactory.createChangeKeyButton(L"up"));

	auto moveDownKey = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"moveDownKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveDownKey->setOnKeyPressed(_uiElementFactory.createChangeKeyButton(L"down"));

	auto audioHeader = _uiElementFactory.createLabel(L"./Config/options.ini",	L"audioHeader");

	auto soundVolumeLabel = _uiElementFactory.createLabel(L"./Config/options.ini", L"soundVolumeLabel");
	auto soundVolumeTextBox = _uiElementFactory.createTextBox(L"./Config/options.ini", L"soundVolumeTextBox");
	soundVolumeTextBox->setName("soundVolume");

	auto musicVolumeLabel = _uiElementFactory.createLabel(L"./Config/options.ini", L"musicVolumeLabel");
	auto musicVolumeTextBox = _uiElementFactory.createTextBox(L"./Config/options.ini", L"musicVolumeTextBox");
	musicVolumeTextBox->setName("musicVolume");

	auto characterHeader = _uiElementFactory.createLabel(L"./Config/options.ini", L"characterHeader");
	auto characterNameLabel = _uiElementFactory.createLabel(L"./Config/options.ini", L"characterNameLabel");

	auto characterNameTextBox = _uiElementFactory.createTextBox(L"./Config/options.ini", L"characterNameTextBox");
	characterNameTextBox->setName("playerName");

	auto appearance = _uiElementFactory.createLabel(L"./Config/options.ini", L"appearance");

	// TODO res loader
	auto currSpriteNumber = ConfigIO::readInt(L"player", L"sprite", 1);
	std::stringstream ss;
	ss << "Resources/Images/Player" << currSpriteNumber << "_single.png";

	auto texture = new sf::Texture();
	if (!texture->loadFromFile(ss.str()))
		LOG_E("Error loading small player texture");
	auto playerImage = new Image(texture);
	auto x = ConfigIO::readInt(L"appearanceImage", L"x", 20, L"./Config/options.ini");
	auto y = ConfigIO::readInt(L"appearanceImage", L"y", 20, L"./Config/options.ini");
	playerImage->setPosition(sf::Vector2f(x, y));
	playerImage->setName("playerImage");

	auto previousAppearance = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"previousAppearance",
		[](UiElement* source, const sf::Event& event)
		{
			auto currSpriteNumber = ConfigIO::readInt(L"player", L"sprite", 1);
			auto totalSprites = ConfigIO::readInt(L"player", L"totalSprites", 1);
			auto newSpriteNumber = (--currSpriteNumber > 0) ? currSpriteNumber : totalSprites;
			ConfigIO::writeInt(L"player", L"sprite", newSpriteNumber);
			
			std::stringstream ss;
			ss << "Resources/Images/Player" << newSpriteNumber << "_single.png";

			auto texture = new sf::Texture();
			if (!texture->loadFromFile(ss.str()))
				LOG_E("Error loading small player texture");

			auto image = GamePhaseManager::getInstance()->getCurrentPhase()
				->getUi().getElementByName("playerImage");
			dynamic_cast<Image*>(image)->changeTexture(texture);
		}
	);

	auto nextAppearance = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"nextAppearance",
		[](UiElement* source, const sf::Event& event)
		{
		auto currSpriteNumber = ConfigIO::readInt(L"player", L"sprite", 1);
		auto totalSprites = ConfigIO::readInt(L"player", L"totalSprites", 1);
		auto newSpriteNumber = (++currSpriteNumber <= totalSprites) ? currSpriteNumber : 1;
		ConfigIO::writeInt(L"player", L"sprite", newSpriteNumber);

		std::stringstream ss;
		ss << "Resources/Images/Player" << newSpriteNumber << "_single.png";

		auto texture = new sf::Texture();
		if (!texture->loadFromFile(ss.str()))
			LOG_E("Error loading small player texture");

		auto image = GamePhaseManager::getInstance()->getCurrentPhase()
			->getUi().getElementByName("playerImage");
		dynamic_cast<Image*>(image)->changeTexture(texture);
		}
	);

	auto back = _uiElementFactory.createButton(
		L"./Config/options.ini",
		L"back",
		[](UiElement* source, const sf::Event& event)
		{
			auto textBox = GamePhaseManager::getInstance()->getCurrentPhase()
				->getUi().getElementByName("playerName");
			auto playerName = dynamic_cast<TextBox*>(textBox)->getText();

			std::wstring playerNameAsWStr;
			playerNameAsWStr.assign(playerName.begin(), playerName.end());

			ConfigIO::writeString(L"characterNameTextBox", L"text", playerNameAsWStr.c_str(), L"./Config/options.ini");
			ConfigIO::writeString(L"player", L"name", playerNameAsWStr.c_str());

			textBox = GamePhaseManager::getInstance()->getCurrentPhase()
				->getUi().getElementByName("soundVolume");
			auto volume = stoi(dynamic_cast<TextBox*>(textBox)->getText());
			ConfigIO::writeInt(L"soundVolumeTextBox", L"text", volume, L"./Config/options.ini");
			ConfigIO::writeInt(L"audio", L"soundVolume", volume);

			textBox = GamePhaseManager::getInstance()->getCurrentPhase()
				->getUi().getElementByName("musicVolume");
			volume = stoi(dynamic_cast<TextBox*>(textBox)->getText());
			ConfigIO::writeInt(L"musicVolumeTextBox", L"text", volume, L"./Config/options.ini");
			ConfigIO::writeInt(L"audio", L"musicVolume", volume);

			Blackboard::getInstance()->leaveCallback(
				AUDIO,
				[](Module* target)
				{
					auto audioModule = dynamic_cast<Audio*>(target);
					audioModule->reloadVolumeFromConfig();
				});

			GamePhaseManager::getInstance()->popPhase();
		});

	auto options = new Menu;
	options->_ui.addElement(controlsHeader);
	options->_ui.addElement(moveLeftKey);
	options->_ui.addElement(moveRightKey);
	options->_ui.addElement(moveUpKey);
	options->_ui.addElement(moveDownKey);
	options->_ui.addElement(audioHeader);
	options->_ui.addElement(soundVolumeLabel);
	options->_ui.addElement(soundVolumeTextBox);
	options->_ui.addElement(musicVolumeLabel);
	options->_ui.addElement(musicVolumeTextBox);
	options->_ui.addElement(characterHeader);
	options->_ui.addElement(characterNameLabel);
	options->_ui.addElement(characterNameTextBox);
	options->_ui.addElement(appearance);
	options->_ui.addElement(playerImage);
	options->_ui.addElement(previousAppearance);
	options->_ui.addElement(nextAppearance);
	options->_ui.addElement(back);

	return options;
}