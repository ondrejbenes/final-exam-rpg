#pragma once

#include "Entity.h"

class EntityFactory;

class Tile : public Entity
{
	friend EntityFactory;
public:
	int tileType;
	bool  _blocking;

	bool isBlocking() const { return _blocking; }

	void changeType(unsigned int newType, bool blocking);

protected:
	Tile();
};