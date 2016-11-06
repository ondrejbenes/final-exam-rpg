#pragma once
#include "Entity.h"

struct CharacterStats
{
	//unsigned agility;
	//unsigned defense;
	unsigned int max_hitpoints;
	unsigned int current_hitpoints;
	//unsigned strength;
};

class EntityFactory;

class Character : public Entity
{
	friend EntityFactory;
public:
	virtual ~Character();
	CharacterStats* getStats() const { return _stats; }
protected:
	explicit Character(CharacterStats stats);
private:
	CharacterStats* _stats;
};