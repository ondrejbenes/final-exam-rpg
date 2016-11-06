#pragma once

#include "Character.h"

class EntityFactory;

class Npc : public Character
{
	friend EntityFactory;
public:
	virtual ~Npc();
protected:
	explicit Npc(CharacterStats stats);

};