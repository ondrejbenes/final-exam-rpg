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

using namespace tinyxml2;

LevelLoader::LevelLoader()
{
	/*XMLDocument doc;
	auto gameData = doc.LoadFile("Resources/levels/EntitiesData.xml");
	XMLElement* titleElement = doc.FirstChildElement("Level")->
		FirstChildElement("Weapons")->FirstChildElement("Weapons_player")->
		FirstChildElement("Weapon_Basic")->FirstChildElement("MinimumDamage");



	const char* HP = titleElement->GetText();
	printf(" %s ", HP);
	XMLElement* title2 = titleElement->NextSiblingElement();
	const char* HP1 = title2->GetText();
	printf(" %s ", HP1);
	auto* nextElement = titleElement->NextSiblingElement()->GetText();
	printf(" %s ", nextElement);


	XMLElement* titleElement2 = doc.FirstChildElement("Level")->FirstChildElement("NPCs")->FirstChildElement();
*/








	/*auto* titleElement2 = doc.FirstChildElement("Level")->FirstChildElement("NPCs")->
		LastChildElement("NPC")->FirstChildElement("Sprite");

	auto* titleElement2Value = titleElement2->FirstAttribute()->Value();
	printf(" 2Name of play : %s", titleElement2Value);
*/
/*auto* nextElement2 = titleElement2->NextSiblingElement();
auto* titleElement3Value = nextElement2->FirstAttribute()->Value();
printf(" 3Name of play : %s", titleElement3Value);*/




	auto a = 0;


}

void LevelLoader::load(const std::string& definitionXml) const
{

	XMLDocument doc;
	EntityFactory Factory;
	auto gameData = doc.LoadFile(definitionXml.c_str());
	auto TileMapElement = doc.FirstChildElement("Level")->FirstChildElement("TileMap");

	auto* SizeXML = TileMapElement->
		FirstChildElement("Size");

	// loads map size
	auto MapsizeXML = SizeXML->FirstChildElement("MapSize");
	Tilemap::MAP_WIDTH = std::stoi(MapsizeXML->Attribute("Width"));
	Tilemap::MAP_HEIGHT = std::stoi(MapsizeXML->Attribute("Height"));
	// loads tile size
	auto TilesizeXML = SizeXML->FirstChildElement("TileSize");
	Tilemap::TILE_WIDTH = std::stoi(TilesizeXML->Attribute("Width"));
	Tilemap::TILE_HEIGHT = std::stoi(TilesizeXML->Attribute("Height"));

	auto* BlockingXML = TileMapElement->
		FirstChildElement("BlockingTilesID");

	//loads blocking tiles
	auto Tokens = StringUtilities::split(BlockingXML->GetText(), ',');
	for (auto Token : Tokens)
	{
		Tilemap::BLOCKING_TILES.push_back(std::stoi(Token));
	}

	// load tiles and add them to EntityManager
	auto MapCSV = TileMapElement->FirstChildElement("Tiles")->Attribute("definition");
	auto TilePNG = TileMapElement->FirstChildElement("Tiles")->Attribute("tiles");

	Tilemap::loadFromFile(TilePNG, MapCSV);

	auto SpecialTile = TileMapElement->FirstChildElement("SpecialTiles")->FirstChildElement("arenaTunnelEntrance");

	//ArenaEntrance
	auto X = SpecialTile->Attribute("x");
	auto Y = SpecialTile->Attribute("y");

	MainGame::arenaTunnelEntrance = sf::Vector2f(std::stoi(X), std::stoi(Y));

	//arenaExit
	SpecialTile = SpecialTile->NextSiblingElement("arenaTunnelExit");

	X = SpecialTile->Attribute("x");
	Y = SpecialTile->Attribute("y");

	MainGame::arenaTunnelExit = sf::Vector2f(std::stoi(X), std::stoi(Y));

	//bronze key
	SpecialTile = SpecialTile->NextSiblingElement("Bronze");

	X = SpecialTile->Attribute("x");
	Y = SpecialTile->Attribute("y");

	MainGame::bronzeKeyGateTilePos = sf::Vector2f(std::stoi(X), std::stoi(Y));

	//Silver Key
	SpecialTile = SpecialTile->NextSiblingElement("Silver");

	X = SpecialTile->Attribute("x");
	Y = SpecialTile->Attribute("y");

	MainGame::silverKeyGateTilePos = sf::Vector2f(std::stoi(X), std::stoi(Y));


	//Gold key
	SpecialTile = SpecialTile->NextSiblingElement("Gold");

	X = SpecialTile->Attribute("x");
	Y = SpecialTile->Attribute("y");

	MainGame::goldKeyGateTilePos = sf::Vector2f(std::stoi(X), std::stoi(Y));

	//Inventory keys
	auto Items = doc.FirstChildElement("Level")->FirstChildElement("Items")->
		FirstChildElement("Keys");

	auto Keys = Items->FirstChildElement("Bronze");
	auto name = Keys->Attribute("name");
	auto image = Keys->Attribute("image");

	auto BronzeKey = Factory.createInventoryItem(image);
	BronzeKey->setName(name);

	Keys = Keys->NextSiblingElement("Silver");
	name = Keys->Attribute("name");
	image = Keys->Attribute("image");

	auto SilverKey = Factory.createInventoryItem(image);
	SilverKey->setName(name);

	Keys = Keys->NextSiblingElement("Gold");
	name = Keys->Attribute("name");
	image = Keys->Attribute("image");

	auto GoldKey = Factory.createInventoryItem(image);
	GoldKey->setName(name);

	auto Weapons = doc.FirstChildElement("Level")->FirstChildElement("Items")->FirstChildElement("Weapons");

	// auto Weapon = Weapons->FirstChildElement("Weapon");

	std::map<std::string, std::shared_ptr<Weapon>> weaponsMap;

	for (auto weapon = Weapons->FirstChild(); weapon; weapon->NextSibling())
	{
		auto Name = weapon->FirstChildElement("Weapon")->Attribute("name");
		auto MinimumDamage = std::stoi(weapon->FirstChildElement("MinimumDamage")->GetText());
		auto MaximumDamage = std::stoi(weapon->FirstChildElement("MaximumDamage")->GetText());
		auto Speed = std::stoi(weapon->FirstChildElement("Speed")->GetText());
		auto Image = weapon->FirstChildElement("Image");		
		auto  weaponItem = std::make_shared<Weapon>(MinimumDamage, MaximumDamage, Speed);
		weaponsMap[Name] = weaponItem;

		if (Image != nullptr)
		{
			auto LinkToImage = Image->Attribute("link");
			Factory.initWeapon(weaponItem, LinkToImage);
		}

	}


	//TODO: Needs a loop	
	auto* PlayerXML = doc.FirstChildElement("Level")->FirstChildElement("Player");
	auto* EnemyXML = doc.FirstChildElement("Level")->FirstChildElement("NPCs");

	// load npcs
	auto usedWeapon = EnemyXML->FirstChildElement("UsedWeapon")->Attribute("name");
	Factory.createNpcFromXml(*EnemyXML, weaponsMap[usedWeapon]);







	// load player
	Factory.createPlayerFromXml(*PlayerXML);

	//





	// create donkey







	
	
}

