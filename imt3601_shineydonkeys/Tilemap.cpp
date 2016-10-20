#include "Tilemap.h"
#include "Logger.h"
#include <fstream>
#include "EntityManager.h"
#include "EntityFactory.h"

Tilemap::Tilemap(): tileSheet(new sf::Texture)
{
	if (!tileSheet->loadFromFile("Resources/Images/180tiles (3).png"))
	LOG_E("Error: could not load tiles texture");
}

Tilemap::~Tilemap()
{

}

int Tilemap::generate()
{

	return 0;
}

bool Tilemap::loadFromFile(std::string fileName)
{
	tiles.clear();
	auto entityManager = EntityManager::getInstance();
	entityManager->clearTiles();

	EntityFactory factory;

	// TODO handle file not found, use ResourceLoader
	std::ifstream input("level02.txt"); 
	auto x = 0;
	for (std::string line; getline(input, line); x++)
	{
		auto tile = factory.create<Tile>();
		tile->tileType = stoi(line) + 7 * (rand() % 7 + 1);
		tile->setPosition(sf::Vector2f(x % rows_x * tile_x, x / rows_y * tile_y));

		sf::Sprite tileImage;
		tileImage.setTexture(*tileSheet);
		tileImage.setTextureRect(sf::IntRect(tile->tileType / 7 * 180 + 1, tile->tileType % 7 * 180 + 1, 180 - 1, 180 - 1));
		tileImage.setPosition(x % rows_x * tile_x, x / rows_y * tile_y);

		auto graphicsComponent = tile->getComponent<GraphicsComponent>();
		graphicsComponent->setSprite(tileImage);
		
		tiles.push_back(tile);
		entityManager->add(tile);
	}

	LOG_D(x + " line loaded");
	input.close();

	return true;
}