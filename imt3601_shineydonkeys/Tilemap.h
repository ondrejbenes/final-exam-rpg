#pragma once
#include "Tile.h"

class Tilemap
{

public:
	Tilemap();
	virtual ~Tilemap();
	Tile *m_Tile;

	int generate();
	int loadFromFile();

};