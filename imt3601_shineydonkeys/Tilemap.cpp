#include "Tilemap.h"
#include "Logger.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "StringUtilities.h"

#include <fstream>

Tilemap::Tilemap()
{
	
}

Tilemap::~Tilemap()
{

}

int Tilemap::generate()
{

	return 0;
}

bool Tilemap::loadFromFile(const std::string& textureFileName, const std::string& levelDefinitionFileName)
{
	auto entityManager = EntityManager::getInstance();
	entityManager->clearTiles();

	std::vector<int> blockingTiles = { 163, 48, 49, 64, 65 };

	EntityFactory factory;

	// TODO shared ptr?s
	auto tileMap = new sf::Texture();
	if (!tileMap->loadFromFile(textureFileName))
		LOG_E("Failed to load TileMap texture");

	auto tilesPerRow = tileMap->getSize().x / TILE_HEIGHT;

	// TODO handle file not found, use ResourceLoader
	std::ifstream input(levelDefinitionFileName);
	auto row = 0;
	auto column = 0;
	for (std::string line; getline(input, line); row++)
	{
		auto types = StringUtilities::split(line, ',');
		for(column = 0; column < types.size(); column++)
		{
			auto tile = factory.create<Tile>();
			auto type = stoi(types[column]);
			tile->tileType = type;

			if (find(begin(blockingTiles), end(blockingTiles), type) != end(blockingTiles))
				tile->_blocking = true;

			auto x = column * TILE_WIDTH;
			auto y = row * TILE_HEIGHT;
			tile->setPosition(sf::Vector2f(x, y));

			auto tileMapRow = floor(type / tilesPerRow);
			auto tileMapColumn = type - (tileMapRow * tilesPerRow);

			sf::Sprite tileImage;
			tileImage.setTexture(*tileMap);
			tileImage.setTextureRect(sf::IntRect(tileMapColumn * TILE_WIDTH, tileMapRow * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT));
			tileImage.setPosition(sf::Vector2f(column * TILE_WIDTH, row * TILE_HEIGHT));

			auto graphicsComponent = tile->getComponent<GraphicsComponent>();
			graphicsComponent->setSprite(tileImage);

			// TODO ugly
			auto& collider = tile->getComponent<PhysicsComponent>()->getCollider();
			collider.height = TILE_HEIGHT;
			collider.width = TILE_WIDTH;
			collider.left = x;
			collider.top = y;

			entityManager->add(tile);
		}
	}

	MAP_WIDTH = TILE_HEIGHT * --row;
	MAP_HEIGHT = TILE_HEIGHT * --column;

	input.close();

	return true;
}

unsigned int Tilemap::TILE_WIDTH = 32;
unsigned int Tilemap::TILE_HEIGHT = 32;

unsigned int Tilemap::MAP_WIDTH = 0;
unsigned int Tilemap::MAP_HEIGHT = 0;