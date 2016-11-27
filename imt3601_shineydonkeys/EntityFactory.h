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
#include "tinyxml2.h"

class EntityFactory
{
public:	
	template <typename T> 
	T* create();

	Entity* createFromToString(std::string str);
	Npc* createNpcFromXml(const tinyxml2::XMLElement& element, const std::shared_ptr<Weapon>& weaponName);
	Player* createPlayerFromXml(const tinyxml2::XMLElement& element);
	Character* createDonkey();
	//Npc* createNpcFromXml(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute& weaponName);
	void initWeapon(std::shared_ptr<Item> weapon, const std::string& texturePath);
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

	auto weapon = std::make_shared<Weapon>(25, 50, 1000);
	npc->setEquipedWeapon(weapon);
	// npc->getInventory().push_back(weapon);

	auto key = createInventoryItem("Resources/Images/Keys/BronzeKey.png");
	key->setName("Bronze Key");
	npc->getInventory().push_back(key);
	auto skey = createInventoryItem("Resources/Images/Keys/SilverKey.png");
	skey->setName("Silver Key");
	npc->getInventory().push_back(skey);
	auto gkey = createInventoryItem("Resources/Images/Keys/GoldKey.png");
	gkey->setName("Gold Key");
	npc->getInventory().push_back(gkey);

	auto gc = new GraphicsComponent(*npc);
	gc->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, 4);
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
	npc->addComponent(gc);

	if (!Network::isMultiplayer() || Network::isServer())
		npc->addComponent(new AiComponent(*npc));

	npc->addComponent(new CombatComponent(*npc));

	auto pc = new PhysicsComponent(*npc);
	auto colliderRect = sf::FloatRect(sprite.getGlobalBounds());
	colliderRect.width /= 4;
	colliderRect.height /= 4;
	pc->setCollider(colliderRect);
	npc->addComponent(pc);

	npc->addComponent(new AnimationComponent(*npc));

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
	ss << "Resources/Images/Char_Animations/Player" << currSpriteNumber << ".png";
	

	auto texture = new sf::Texture();
	if (!texture->loadFromFile(ss.str()))
		LOG_E("Error loading player texture");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	ss.seekp(-4, ss.cur);
	ss << "_Battle.png";

	auto combatTexture = new sf::Texture;
	if (!combatTexture->loadFromFile(ss.str()))
		LOG_E("Error: could not load player image");
	sf::Sprite combatSprite;
	combatSprite.setTexture(*combatTexture);
	
	auto sword = std::make_shared<Weapon>(25, 50, 1000);
	initWeapon(sword, "Resources/Images/Weapons/Sword.png");
	player->setEquipedWeapon(sword);
	player->getInventory().push_back(sword);

	auto axe = std::make_shared<Weapon>(75, 125, 1500);
	initWeapon(axe, "Resources/Images/Weapons/Axe.png");
	player->getInventory().push_back(axe);

	//player->getInventory().push_back(createInventoryItem("Resources/Images/Keys/SilverKey.png"));
	//player->getInventory().push_back(createInventoryItem("Resources/Images/Keys/GoldKey.png"));

	auto gc = new GraphicsComponent(*player);
	gc->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, 4);
	gc->addSprite(CombatComponent::COMBAT_SPRITE_NAME, combatSprite, 4);
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	player->addComponent(gc);
	player->addComponent(new CombatComponent(*player));
	
	auto pc = new PhysicsComponent(*player);
	auto colliderRect = sf::FloatRect(sprite.getGlobalBounds());
	colliderRect.width /= 4;
	colliderRect.height /= 4;
	pc->setCollider(colliderRect);
	player->addComponent(pc);

	player->addComponent(new AnimationComponent(*player));

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