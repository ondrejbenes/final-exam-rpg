#pragma once
#include "Tile.h"
#include <SFML/Graphics/Texture.hpp>

class Tilemap
{

public:
	std::vector<Tile*> tiles;

	Tilemap();
	virtual ~Tilemap();

	int generate();
	bool loadFromFile(std::string fileName);

	int tile_x = 110;
	int tile_y = 75;
	int rows_x = 44;
	int rows_y = 44;

	sf::Texture* tileSheet;
};
