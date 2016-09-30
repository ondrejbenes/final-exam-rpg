#pragma once
#include "Entity.h"

class EntityFactory;

class Tile : public Entity
{
	friend EntityFactory;
public:
	int tileType;

	virtual ~Tile();

protected:
	Tile();
};