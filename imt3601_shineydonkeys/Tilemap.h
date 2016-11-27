#pragma once

#include <string>
#include <vector>

class Tilemap
{
public:
	static const std::string TILE_SPRITE_NAME;

	static unsigned int TILE_WIDTH;
	static unsigned int TILE_HEIGHT;

	static unsigned int MAP_WIDTH;
	static unsigned int MAP_HEIGHT;

	static std::vector<unsigned> BLOCKING_TILES;

	Tilemap();
	virtual ~Tilemap();

	static bool loadFromFile(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);
};
