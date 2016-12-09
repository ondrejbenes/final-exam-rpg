#include "Tilemap.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "StringUtilities.h"
#include "ResourceLoader.h"

#include <fstream>

bool Tilemap::loadFromFile(const std::string& textureFileName, const std::string& levelDefinitionFileName)
{
	auto entityManager = EntityManager::getInstance();
	entityManager->clearTiles();
	
	EntityFactory factory;

	auto tileMap = ResourceLoader::getInstance()->getTexture(textureFileName);

	auto tilesPerRow = tileMap->getSize().x / TILE_HEIGHT;

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

			if (find(begin(BLOCKING_TILES), end(BLOCKING_TILES), type) != end(BLOCKING_TILES))
				tile->_blocking = true;

			auto x = column * TILE_WIDTH;
			auto y = row * TILE_HEIGHT;

			auto tileMapRow = floor(type / tilesPerRow);
			auto tileMapColumn = type - (tileMapRow * tilesPerRow);

			sf::Sprite tileImage;
			tileImage.setTexture(*tileMap);
			tileImage.setTextureRect(sf::IntRect(tileMapColumn * TILE_WIDTH, tileMapRow * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT));
			tileImage.setPosition(sf::Vector2f(column * TILE_WIDTH, row * TILE_HEIGHT));

			auto graphicsComponent = tile->getComponent<GraphicsComponent>();
			graphicsComponent->addSprite(TILE_SPRITE_NAME, tileImage);
			graphicsComponent->setActiveSprite(TILE_SPRITE_NAME);

			sf::FloatRect collider;
			collider.height = TILE_HEIGHT;
			collider.width = TILE_WIDTH;
			collider.left = x;
			collider.top = y;

			tile->getComponent<PhysicsComponent>()->setCollider(collider);

			tile->setPosition(sf::Vector2f(x, y));
			entityManager->add(tile);
		}
	}
	// TODO need to load map size from XML file before we add tiles to EM
	MAP_WIDTH = TILE_WIDTH * --column;
	MAP_HEIGHT = TILE_HEIGHT * --row;

	input.close();

	return true;
}

const std::string Tilemap::TILE_SPRITE_NAME = "tile";

unsigned int Tilemap::TILE_WIDTH = 32;
unsigned int Tilemap::TILE_HEIGHT = 32;

unsigned int Tilemap::MAP_WIDTH = 0;
unsigned int Tilemap::MAP_HEIGHT = 0;

std::vector<unsigned> Tilemap::BLOCKING_TILES = { 73, 163, 48, 49, 64, 65 };