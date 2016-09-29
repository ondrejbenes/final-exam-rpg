#pragma once
#include "Tile.h"

class Tilemap
{

public:
	std::vector<Tile*> tiles;

	Tilemap();
	virtual ~Tilemap();

	int generate();
	bool loadFromFile(std::string fileName);

};