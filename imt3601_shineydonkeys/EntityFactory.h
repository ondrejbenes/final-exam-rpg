#pragma once

#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Logger.h"
#include "Tile.h"
#include "AiComponent.h"
#include "AnimationComponent.h"
#include "CombatComponent.h"
#include "Weapon.h"
#include "Network.h"
#include "ConfigIO.h"

#include <sstream>

class EntityFactory
{
public:
	EntityFactory();
	virtual ~EntityFactory();

	Entity* createFromToString(std::string str);
	void initWeapon(std::shared_ptr<Item> weapon, const std::string& texturePath);
	template <typename T> 
	T* create();

	std::shared_ptr<Item> createInventoryItem(const std::string& texturePath);
};

template <typename T>
T* EntityFactory::create()
{
	throw UnsopportedEntityException(typeid(T).name());
}

template <>
inline Npc* EntityFactory::create<Npc>()
{
	CharacterStats stats;
	stats.max_hitpoints = 300;
	stats.current_hitpoints = 300;
	auto npc = new Npc(stats);

	// TODO ResLoader
	auto texture = new sf::Texture;
	if (!texture->loadFromFile("Resources/Images/Npc1.png"))
		LOG_E("Error: could not load player image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto weapon = std::make_shared<Weapon>(10, 20, 1000);
	npc->setEquipedWeapon(weapon);
	// npc->getInventory().push_back(weapon);

	auto key = createInventoryItem("Resources/Images/Keys/BronzeKey.png");
	key->setName("Bronze Key");
	npc->getInventory().push_back(key);

	auto gc = new GraphicsComponent(*npc);
	gc->setSprite(sprite);
	npc->addComponent(gc);

	if (!Network::isMultiplayer() || Network::isServer())
		npc->addComponent(new AiComponent(*npc));

	npc->addComponent(new AnimationComponent(*npc));
	npc->addComponent(new CombatComponent(*npc));
	npc->addComponent(new PhysicsComponent(*npc));

	return npc;
}

template <>
inline Player* EntityFactory::create<Player>()
{
	CharacterStats stats;
	stats.max_hitpoints = 500;
	stats.current_hitpoints = 500;

	auto player = new Player(stats);

	// TODO ResLoader
	auto currSpriteNumber = ConfigIO::readInt(L"player", L"sprite", 1);
	std::stringstream ss;
	ss << "Resources/Images/Player" << currSpriteNumber << ".png";

	auto texture = new sf::Texture();
	if (!texture->loadFromFile(ss.str()))
		LOG_E("Error loading player texture");
	sf::Sprite sprite;
	sprite.setTexture(*texture);
	
	auto sword = std::make_shared<Weapon>(100, 200, 1000);
	initWeapon(sword, "Resources/Images/Weapons/Sword.png");
	player->setEquipedWeapon(sword);
	player->getInventory().push_back(sword);

	auto axe = std::make_shared<Weapon>(50, 75, 1500);
	initWeapon(axe, "Resources/Images/Weapons/Axe.png");
	player->getInventory().push_back(axe);

	//player->getInventory().push_back(createInventoryItem("Resources/Images/Keys/SilverKey.png"));
	//player->getInventory().push_back(createInventoryItem("Resources/Images/Keys/GoldKey.png"));

	auto gc = new GraphicsComponent(*player);
	gc->setSprite(sprite);
	player->addComponent(gc);
	player->addComponent(new AnimationComponent(*player));
	player->addComponent(new CombatComponent(*player));
	player->addComponent(new PhysicsComponent(*player));

	return player;
}

template <>
inline Tile* EntityFactory::create<Tile>()
{
	auto tile = new Tile();
	tile->addComponent(new GraphicsComponent(*tile));
	tile->addComponent(new PhysicsComponent(*tile, true));

	return tile;
}