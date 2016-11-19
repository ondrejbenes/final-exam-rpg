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
	if (!texture->loadFromFile("Resources/Images/minimap.png"))
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

	chatBoard->addMessage("System", "Welcome to Final Exam!");

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

	auto singlePlayer = createButton(
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

	auto startMP = createButton(
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

	auto joinMP = createButton(
		configFile,
		L"joinMP",
		[&](UiElement* source, const sf::Event& event)
		{ GamePhaseManager::getInstance()->pushPhase(createJoinMultiPlayerGame()); });

	auto options = createButton(
		configFile,
		L"options",
		[&](UiElement* source, const sf::Event& event) 
		{ GamePhaseManager::getInstance()->pushPhase(createOptions()); });

	auto exit = createButton(
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

	auto ip = createLabel(configFile, L"ip");

	auto ipStr = "Your IP is: " + sf::IpAddress::getLocalAddress().toString();
	auto ipAsBtn = dynamic_cast<Label*>(ip);
	ipAsBtn->setText(ipStr);

	auto spectratorsHeader = createLabel(configFile, L"spectratorsHeader");

	auto start = createButton(
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

	auto back = createButton(
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

	auto ip = createLabel(configFile, L"serverIpLabel");

	auto serverIpTextBox = createTextBox(configFile, L"serverIpTextBox");
	serverIpTextBox->setName("serverIp");

	auto connect = createButton(
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

	auto back = createButton(
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
	auto controlsHeader = createLabel(L"./Config/options.ini", L"controlsHeader");

	auto moveLeftKey = createButton(
		L"./Config/options.ini",
		L"moveLeftKey",
		[](UiElement* source, const sf::Event& event) 
		{ /* only set this element as focused*/ });
	moveLeftKey->setOnKeyPressed(createChangeKeyButton(L"left"));

	auto moveRightKey = createButton(
		L"./Config/options.ini",
		L"moveRightKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveRightKey->setOnKeyPressed(createChangeKeyButton(L"right"));

	auto moveUpKey = createButton(
		L"./Config/options.ini",
		L"moveUpKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveUpKey->setOnKeyPressed(createChangeKeyButton(L"up"));

	auto moveDownKey = createButton(
		L"./Config/options.ini",
		L"moveDownKey",
		[](UiElement* source, const sf::Event& event)
	{ /* only set this element as focused*/ });
	moveDownKey->setOnKeyPressed(createChangeKeyButton(L"down"));

	auto audioHeader = createLabel(L"./Config/options.ini",	L"audioHeader");

	auto soundVolumeLabel = createLabel(L"./Config/options.ini", L"soundVolumeLabel");
	auto soundVolumeTextBox = createTextBox(L"./Config/options.ini", L"soundVolumeTextBox");
	soundVolumeTextBox->setName("soundVolume");

	auto musicVolumeLabel = createLabel(L"./Config/options.ini", L"musicVolumeLabel");
	auto musicVolumeTextBox = createTextBox(L"./Config/options.ini", L"musicVolumeTextBox");
	musicVolumeTextBox->setName("musicVolume");

	auto characterHeader = createLabel(L"./Config/options.ini", L"characterHeader");
	auto characterNameLabel = createLabel(L"./Config/options.ini", L"characterNameLabel");

	auto characterNameTextBox = createTextBox(L"./Config/options.ini", L"characterNameTextBox");
	characterNameTextBox->setName("playerName");

	auto appearance = createLabel(L"./Config/options.ini", L"appearance");

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

	auto previousAppearance = createButton(
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

	auto nextAppearance = createButton(
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

	auto back = createButton(
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

// TODO make label a member of button class?
Button* GamePhaseFactory::createButton(const wchar_t* configFile, const wchar_t* configSectionName, UiCallback onClick)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new Button(text, font, fontSize);
	uiElement->setOnClick(new UiCallback(onClick));
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

Label* GamePhaseFactory::createLabel(const wchar_t* configFile, const wchar_t* configSectionName) 
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new Label(text, font, fontSize);
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

TextBox* GamePhaseFactory::createTextBox(const wchar_t* configFile, const wchar_t* configSectionName)
{
	auto text = ConfigIO::readString(configSectionName, L"text", L"???", configFile);
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = ConfigIO::readInt(configSectionName, L"fontSize", 20, configFile);
	auto x = ConfigIO::readInt(configSectionName, L"x", 20, configFile);
	auto y = ConfigIO::readInt(configSectionName, L"y", 20, configFile);

	auto uiElement = new TextBox(font, text, fontSize);
	uiElement->setPosition(sf::Vector2f(x, y));

	return uiElement;
}

UiCallback* GamePhaseFactory::createChangeKeyButton(const std::wstring& moveDirection)
{
	auto lambda = [moveDirection](UiElement* source, const sf::Event& event)
	{
		auto sfCode = event.key.code;
		ConfigIO::writeInt(L"controls", moveDirection.c_str(), sfCode);

		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;

		auto humanReadable = new char[20];
		StringUtilities::SFKeyToString(sfCode, humanReadable);
		std::stringstream ss;
		ss << "Move " << converter.to_bytes(moveDirection) << ": " << humanReadable;
		delete humanReadable;

		auto btnText = ss.str();
		auto sourceAsBtn = dynamic_cast<Button*>(source);
		sourceAsBtn->setText(btnText);

		std::wstring btnTextAsWStr;
		btnTextAsWStr.assign(btnText.begin(), btnText.end());

		std::wstringstream wss;
		wss << L"move" << wchar_t(towupper(moveDirection[0])) << std::nouppercase << moveDirection.substr(1) << L"Key";

		ConfigIO::writeString(wss.str().c_str(), L"text", btnTextAsWStr.c_str(), L"./Config/options.ini");
	};

	return new UiCallback(lambda);
}