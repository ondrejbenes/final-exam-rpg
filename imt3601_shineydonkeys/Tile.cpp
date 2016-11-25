#include "Tile.h"
#include "GraphicsComponent.h"
#include "Tilemap.h"

Tile::Tile() : tileType(-1), _blocking(false)
{

}

void Tile::changeType(unsigned newType, bool blocking) 
{
	auto gc = getComponent<GraphicsComponent>();

	auto& sprite = gc->getActiveSprite();
	auto tilesPerRow = sprite.getTexture()->getSize().x / Tilemap::TILE_HEIGHT;
	
	auto tileMapRow = floor(newType / tilesPerRow);
	auto tileMapColumn = newType - (tileMapRow * tilesPerRow);

	sprite.setTextureRect(sf::IntRect(tileMapColumn * Tilemap::TILE_WIDTH, tileMapRow * Tilemap::TILE_HEIGHT, Tilemap::TILE_WIDTH, Tilemap::TILE_HEIGHT));

	_blocking = blocking;
}
