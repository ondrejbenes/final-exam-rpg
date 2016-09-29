#pragma once
#include "Entity.h"

class Tile : public Entity
{

public:
	int tileType;

	explicit Tile(int tileType);
	virtual ~Tile();

};