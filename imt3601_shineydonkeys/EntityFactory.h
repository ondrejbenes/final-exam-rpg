#pragma once
#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"

class EntityFactory
{
public:
	EntityFactory();
	virtual ~EntityFactory();

	template <typename T> 
	T* create();

	template <>
	Npc* create();
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
	return new Player;
}