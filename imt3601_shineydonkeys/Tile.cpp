#include "Tile.h"
#include "GraphicsComponent.h"
#include "Tilemap.h"
#include "Network.h"
#include "Blackboard.h"

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

	auto id = this->id;

	// TODO not really nice to have it here
	if (Network::isServer())
	{
		Blackboard::getInstance()->leaveCallback(
			NETWORK,
			[id, newType, blocking](Module* target)
		{
			PacketFactory factory;
			auto packet = factory.createChangeTileSprite(id, newType, blocking);
			dynamic_cast<Network*>(target)->broadcast(packet);
		}
		);
	}
}
