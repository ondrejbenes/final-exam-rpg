#pragma once
#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"
#include "PlayerInputComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "SoundComponent.h"

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
	player->addComponent(new GraphicsComponent(*player));
	player->addComponent(new PhysicsComponent(*player));
	player->addComponent(new SoundComponent(*player));

	return player;
}