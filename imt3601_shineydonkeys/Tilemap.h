#pragma once

#include <string>

class Tilemap
{
public:
	static const std::string TILE_SPRITE_NAME;

	static unsigned int TILE_WIDTH;
	static unsigned int TILE_HEIGHT;

	static unsigned int MAP_WIDTH;
	static unsigned int MAP_HEIGHT;

	Tilemap();
	virtual ~Tilemap();

	int generate();
	bool loadFromFile(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);
};
