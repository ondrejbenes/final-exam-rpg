#pragma once

#include "Character.h"

class EntityFactory;

class Npc : public Character
{
	friend EntityFactory;
protected:
	explicit Npc(CharacterStats stats);
};