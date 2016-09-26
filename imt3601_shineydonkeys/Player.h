#pragma once
#include "Character.h"

class Entity;

class Player : public Character
{

public:
	Player();
	virtual ~Player();

private:
	std::string name;

};