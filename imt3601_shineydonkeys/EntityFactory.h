#pragma once

#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"
#include "Logger.h"
#include "Tile.h"
#include "AiComponent.h"
#include "AnimationComponent.h"
#include "CombatComponent.h"
#include "Weapon.h"

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
	auto npc = new Npc;

	// TODO ResLoader
	auto texture = new sf::Texture;
	if (!texture->loadFromFile("Resources/Images/Npc1.png"))
		LOG_E("Error: could not load player image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*npc);
	gc->setSprite(sprite);

	npc->addComponent(gc);
	npc->addComponent(new AiComponent(*npc));
	npc->addComponent(new AnimationComponent(*npc));
	npc->addComponent(new CombatComponent(*npc));
	npc->addComponent(new PhysicsComponent(*npc));
	npc->addComponent(new SoundComponent(*npc));

	auto weapon = new Weapon(15, 25, 200);
	npc->getChildren().push_back(*weapon);

	return npc;
}

template <>
inline Player* EntityFactory::create<Player>()
{
	auto player = new Player;

	// TODO ResLoader
	auto texture = new sf::Texture;
	if (!texture->loadFromFile("Resources/Images/Player2.png"))
		LOG_E("Error: could not load player image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*player);
	gc->setSprite(sprite);

	player->addComponent(gc);
	player->addComponent(new AnimationComponent(*player));
	player->addComponent(new CombatComponent(*player));
	player->addComponent(new PhysicsComponent(*player));
	player->addComponent(new SoundComponent(*player));

	auto weapon = new Weapon(25, 50, 200);
	player->getChildren().push_back(*weapon);

	return player;
}

template <>
inline Tile* EntityFactory::create<Tile>()
{
	auto tile = new Tile();
	tile->addComponent(new GraphicsComponent(*tile));
	tile->addComponent(new PhysicsComponent(*tile));
	tile->addComponent(new SoundComponent(*tile));

	return tile;
}