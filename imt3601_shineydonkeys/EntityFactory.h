#pragma once
#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"
#include "PlayerInputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"
#include "Logger.h"
#include "Tile.h"

class EntityFactory
{
public:
	EntityFactory();
	virtual ~EntityFactory();

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
	return new Npc;
}

template <>
inline Player* EntityFactory::create<Player>()
{
	auto player = new Player;
	player->addComponent(new PlayerInputComponent(*player));

	// TODO ResLoader
	auto texture = new sf::Texture;
	if (!texture->loadFromFile("Resources/Images/noaxereally.png"))
		LOG_E("Error: could not load player image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*player);
	gc->setSprite(sprite);

	player->addComponent(gc);
	player->addComponent(new PhysicsComponent(*player));
	player->addComponent(new SoundComponent(*player));

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