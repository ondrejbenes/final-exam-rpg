#pragma once
#include "Entity.h"

struct CharacterStats
{
	unsigned __int8 agility;
	unsigned __int8 defense;
	unsigned __int8 hitpoints;
	unsigned __int8 strength;
};

class EntityFactory;

class Character : public Entity
{
	friend EntityFactory;
public:
	virtual ~Character();
protected:
	Character();

};