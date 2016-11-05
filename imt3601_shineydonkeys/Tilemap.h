#pragma once

#include <SFML/Graphics/Texture.hpp>

class Tilemap
{
public:
	static unsigned int TILE_WIDTH;
	static unsigned int TILE_HEIGHT;

	Tilemap();
	virtual ~Tilemap();

	int generate();
	bool loadFromFile(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);
};
