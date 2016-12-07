#pragma once
#include "Character.h"

class Entity;
class EntityFactory;

class Player : public Character
{
	friend EntityFactory;

protected:
	explicit Player(CharacterStats stats);
};