#include "Tilemap.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "StringUtilities.h"
#include "ResourceLoader.h"

#include <fstream>
#include <set>

bool Tilemap::loadFromFile(const std::string& textureFileName, const std::string& levelDefinitionFileName)
{
	EntityFactory factory;
	auto entityManager = EntityManager::getInstance();
	entityManager->clearTiles();

	auto tileMap = ResourceLoader::getInstance()->getTexture(textureFileName);
	auto tilesPerRow = tileMap->getSize().x / TILE_HEIGHT;
	std::ifstream input(levelDefinitionFileName);
	auto row = 0;

	for (std::string line; getline(input, line); row++)
	{
		auto types = StringUtilities::split(line, ',');
		for(auto column = 0; column < types.size(); column++)
		{
			auto type = stoi(types[column]);
			auto tile = new Tile(type);

			if (find(begin(BLOCKING_TILES), end(BLOCKING_TILES), type) != end(BLOCKING_TILES))
				tile->blocking = true;

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

	input.close();

	return true;
}

const std::string Tilemap::TILE_SPRITE_NAME = "tile";

unsigned int Tilemap::TILE_WIDTH = 32;
unsigned int Tilemap::TILE_HEIGHT = 32;

unsigned int Tilemap::MAP_WIDTH = 0;
unsigned int Tilemap::MAP_HEIGHT = 0;

std::set<unsigned> Tilemap::BLOCKING_TILES = {};