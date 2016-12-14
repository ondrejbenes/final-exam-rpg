#include "LevelLoader.h"
#include "tinyxml2.h"
#include "tinyxml2.h"
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "Tilemap.h"
#include "StringUtilities.h"
#include "MainGame.h"
#include "EntityFactory.h"
#include "Weapon.h"
#include "EntityManager.h"
#include "Scheduler.h"
#include "Blackboard.h"
#include "ChatBoard.h"
#include "GamePhaseManager.h"

using namespace tinyxml2;
using namespace std::chrono_literals;

void LevelLoader::load(const std::string& definitionXml) const
{
	auto entityManager = EntityManager::getInstance();
	Entity::nextId = 0;

	XMLDocument doc;
	doc.LoadFile(definitionXml.c_str());

	loadMap(doc);
	loadKeys(doc);
	loadWeapons(doc);

	entityManager->clearCharacters();
	loadPlayer(doc);
	loadNpcs(doc);
	loadDonkey();
}

void LevelLoader::loadMap(const XMLDocument& doc) const {
	auto tileMapElement = doc.FirstChildElement("Level")->FirstChildElement("TileMap");

	auto* sizeXML = tileMapElement->FirstChildElement("Size");

	// loads map size
	auto mapsizeXML = sizeXML->FirstChildElement("MapSize");
	Tilemap::MAP_WIDTH = std::stoi(mapsizeXML->Attribute("Width"));
	Tilemap::MAP_HEIGHT = std::stoi(mapsizeXML->Attribute("Height"));
	// loads tile size
	auto tilesizeXML = sizeXML->FirstChildElement("TileSize");
	Tilemap::TILE_WIDTH = std::stoi(tilesizeXML->Attribute("Width"));
	Tilemap::TILE_HEIGHT = std::stoi(tilesizeXML->Attribute("Height"));

	auto* blockingXML = tileMapElement->FirstChildElement("BlockingTilesID");

	//loads blocking tiles
	auto Tokens = StringUtilities::split(blockingXML->GetText(), ',');
	for (auto token : Tokens)
	{
		Tilemap::BLOCKING_TILES.insert(stoi(token));
	}

	// load tiles and add them to EntityManager
	auto mapCsv = tileMapElement->FirstChildElement("Tiles")->Attribute("definition");
	auto tilePNG = tileMapElement->FirstChildElement("Tiles")->Attribute("tiles");

	Tilemap::loadFromFile(tilePNG, mapCsv);

	auto specialTile = tileMapElement->FirstChildElement("SpecialTiles")->FirstChildElement("arenaTunnelEntrance");

	//ArenaEntrance
	auto x = specialTile->Attribute("x");
	auto y = specialTile->Attribute("y");

	MainGame::arenaTunnelEntrance = sf::Vector2f(std::stoi(x), std::stoi(y));

	//arenaExit
	specialTile = specialTile->NextSiblingElement("arenaTunnelExit");

	x = specialTile->Attribute("x");
	y = specialTile->Attribute("y");

	MainGame::arenaTunnelExit = sf::Vector2f(std::stoi(x), std::stoi(y));

	//bronze key
	specialTile = specialTile->NextSiblingElement("Bronze");

	x = specialTile->Attribute("x");
	y = specialTile->Attribute("y");

	MainGame::bronzeKeyGateTilePos = sf::Vector2f(std::stoi(x), std::stoi(y));

	//Silver Key
	specialTile = specialTile->NextSiblingElement("Silver");

	x = specialTile->Attribute("x");
	y = specialTile->Attribute("y");

	MainGame::silverKeyGateTilePos = sf::Vector2f(std::stoi(x), std::stoi(y));


	//Gold key
	specialTile = specialTile->NextSiblingElement("Gold");

	x = specialTile->Attribute("x");
	y = specialTile->Attribute("y");

	MainGame::goldKeyGateTilePos = sf::Vector2f(std::stoi(x), std::stoi(y));
}

void LevelLoader::loadKeys(const XMLDocument& doc) const {
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();

	auto items = doc.FirstChildElement("Level")->FirstChildElement("Items")->
		FirstChildElement("Keys");

	// TODO remove duplication
	auto keys = items->FirstChildElement("Bronze");
	auto name = keys->Attribute("name");
	auto image = keys->Attribute("image");

	auto bronzeKey = factory.createInventoryItem(image);
	bronzeKey->setName(name);
	entityManager->addItem(bronzeKey);

	keys = keys->NextSiblingElement("Silver");
	name = keys->Attribute("name");
	image = keys->Attribute("image");

	auto silverKey = factory.createInventoryItem(image);
	silverKey->setName(name);
	entityManager->addItem(silverKey);

	keys = keys->NextSiblingElement("Gold");
	name = keys->Attribute("name");
	image = keys->Attribute("image");

	auto goldKey = factory.createInventoryItem(image);
	goldKey->setName(name);
	entityManager->addItem(goldKey);
}

void LevelLoader::loadWeapons(const XMLDocument& doc) const {
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();

	auto weapons = doc.FirstChildElement("Level")->FirstChildElement("Items")->FirstChildElement("Weapons");

	for (auto weapon = weapons->FirstChild(); weapon; weapon = weapon->NextSibling())
	{
		auto weaponName = weapon->ToElement()->Attribute("name");
		auto minimumDamage = std::stoi(weapon->FirstChildElement("MinimumDamage")->GetText());
		auto maximumDamage = std::stoi(weapon->FirstChildElement("MaximumDamage")->GetText());
		auto speed = std::stoi(weapon->FirstChildElement("Speed")->GetText());
		auto weaponImage = weapon->FirstChildElement("Image");
		auto  weaponItem = std::make_shared<Weapon>(minimumDamage, maximumDamage, speed);
		weaponItem->setName(weaponName);

		if (weaponImage != nullptr)
		{
			auto LinkToImage = weaponImage->Attribute("link");
			factory.addWeaponInventorySprite(weaponItem, LinkToImage);
		}

		entityManager->addItem(weaponItem);
	}
}

void LevelLoader::loadPlayer(const XMLDocument& doc) const
{
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();

	// load player
	auto* playerXml = doc.FirstChildElement("Level")->FirstChildElement("Player");
	auto player = factory.createPlayerFromXml(*playerXml);
	entityManager->add(player);
	entityManager->setLocalPlayer(player);
}

void LevelLoader::loadNpcs(const XMLDocument& doc) const 
{
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();

	auto npcs = doc.FirstChildElement("Level")->FirstChildElement("NPCs");


	
	for (auto npcXml = npcs->FirstChildElement("NPC"); npcXml; npcXml = npcXml->NextSiblingElement("NPC"))
	{
		auto npc = factory.createNpcFromXml(*npcXml);
		entityManager->add(npc);
	}
}

void LevelLoader::loadDonkey() const 
{
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();

	auto donkey = factory.createDonkey();
	donkey->setPosition(sf::Vector2f(724, 1555));
	donkey->setName("donkey");

	auto boundaryPos = donkey->getPosition() - sf::Vector2f(100, 100);
	auto donkeySpriteBounds = donkey->getComponent<GraphicsComponent>()->getActiveSprite().getGlobalBounds();
	auto boundarySize = sf::Vector2f(donkeySpriteBounds.width + 200, donkeySpriteBounds.height + 200);
	auto donkeyTextTrigger = std::make_shared<Trigger>(sf::FloatRect(boundaryPos, boundarySize));
	auto callback = std::make_shared<std::function<void(Entity*)>>(
		[](Entity* enteringEntity)
	{
		if (typeid(*enteringEntity) != typeid(Player))
			return;

		if (MainGame::donkeyTextShown)
			return;

		MainGame::donkeyTextShown = true;

		auto mainGame = GamePhaseManager::getInstance()->getCurrentPhase();

		auto chatBoard = mainGame->getUi().getElementByName<ChatBoard>("chatBoard");

		chatBoard->addMessage("Shiny Donkey", "Hi. I'm the Shiney Donkey");

		Blackboard::getInstance()->leaveCallback(
			SCHEDULER,
			[chatBoard](Module* target)
		{
			auto scheduler = dynamic_cast<Scheduler*>(target);
			auto msg1 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You won't believe it, but I have a quest for you."); };
			scheduler->schedule(msg1, NOW + 2s);
			auto msg2 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You need to explore the island and look for keys."); };
			scheduler->schedule(msg2, NOW + 4s);
			auto msg3 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "The keys will unlock the gate to the east of here."); };
			scheduler->schedule(msg3, NOW + 6s);
			auto msg4 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "Then I need you to enter the tunnel."); };
			scheduler->schedule(msg4, NOW + 8s);
			auto msg5 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You will know what to do when you exit on the other side."); };
			scheduler->schedule(msg5, NOW + 10s);
		}
		);
	});
	donkeyTextTrigger->setOnTriggerEnter(callback);
	donkey->getComponent<PhysicsComponent>()->getTriggers().push_back(donkeyTextTrigger);
	entityManager->add(donkey);
}
