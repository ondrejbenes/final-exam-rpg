#pragma once

#include "Entity.h"

class EntityFactory;

class Tile : public Entity
{
	friend EntityFactory;
public:
	int tileType;
	bool  _blocking;

	virtual ~Tile();

	bool isBlocking() const { return _blocking; }

protected:
	Tile();

// private:
};