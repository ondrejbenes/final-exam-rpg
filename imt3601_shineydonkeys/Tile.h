#pragma once

#include "Entity.h"

class EntityFactory;

class Tile : public Entity
{
	friend EntityFactory;
public:
	int tileType;
	bool  blocking;

	explicit Tile(int tilyType, bool blocking = false);

	void changeType(unsigned int newType, bool blocking);
};