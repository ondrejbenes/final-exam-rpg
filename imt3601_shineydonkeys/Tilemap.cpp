#include "Tilemap.h"
#include "Logger.h"
#include <fstream>
#include "EntityManager.h"
#include "EntityFactory.h"
#include "StringUtilities.h"

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

	EntityFactory factory;

	// TODO shared ptr?s
	auto tileMap = new sf::Texture();
	if (!tileMap->loadFromFile(textureFileName))
		LOG_E("Failed to load TileMap texture");

	auto tilesPerRow = tileMap->getSize().x / TILE_HEIGHT;

	// TODO handle file not found, use ResourceLoader
	std::ifstream input(levelDefinitionFileName);
	auto row = 0;
	for (std::string line; getline(input, line); row++)
	{
		auto types = StringUtilities::split(line, ',');
		for(auto i = 0; i < types.size(); i++)
		{
			auto tile = factory.create<Tile>();
			auto type = stoi(types[i]);
			tile->tileType = type;
			tile->setPosition(sf::Vector2f(i * TILE_WIDTH, row * TILE_HEIGHT));

			auto tileMapRow = floor(type / tilesPerRow);
			auto tileMapColumn = type - (tileMapRow * tilesPerRow);

			sf::Sprite tileImage;
			tileImage.setTexture(*tileMap);
			tileImage.setTextureRect(sf::IntRect(tileMapColumn * TILE_WIDTH, tileMapRow * TILE_HEIGHT, TILE_WIDTH, TILE_HEIGHT));
			tileImage.setPosition(sf::Vector2f(i * TILE_WIDTH, row * TILE_HEIGHT));

			auto graphicsComponent = tile->getComponent<GraphicsComponent>();
			graphicsComponent->setSprite(tileImage);

			entityManager->add(tile);
		}
	}

	input.close();

	return true;
}

unsigned int Tilemap::TILE_WIDTH = 32;
unsigned int Tilemap::TILE_HEIGHT = 32;