#include "Tilemap.h"
#include "Logger.h"
#include <fstream>
#include "EntityManager.h"

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

bool Tilemap::loadFromFile(std::string fileName)
{
	tiles.clear();
	EntityManager::currentLevelTiles.clear();
	// TODO handle file not found, use ResourceLoader
	std::ifstream input("level02.txt"); 
	auto x = 0;
	for (std::string line; getline(input, line); )
	{
		auto tile = new Tile(stoi(line) + 7 * (rand() % 7 + 1));
		tiles.push_back(tile);
		EntityManager::currentLevelTiles.push_back(tile);
		x++;
	}
	LOG_D(x + " line loaded");
	input.close();

	return true;
}