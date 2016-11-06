#pragma once
#include "Character.h"

class Entity;
class EntityFactory;

class Player : public Character
{
	friend EntityFactory;

public:
	virtual ~Player();

protected:
	explicit Player(CharacterStats stats);

private:
	std::string name;

};