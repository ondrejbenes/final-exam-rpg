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

	template <typename T> 
	T* create();
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

	auto weapon = new Weapon(50, 80, 1000);
	npc->getChildren().push_back(weapon);

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
	
	auto weapon = new Weapon(25, 50, 1000);
	player->getChildren().push_back(weapon);

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