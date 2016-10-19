#include "EntityManager.h"

EntityManager::EntityManager()
{

}

EntityManager::~EntityManager()
{
	/*
	for (auto it = gameEntities.begin(); it != gameEntities.end(); ++it)
		delete *it;

	for (auto it = currentLevelEntities.begin(); it != currentLevelEntities.end(); ++it)
		delete *it;

	for (auto it = currentLevelTiles.begin(); it != currentLevelTiles.end(); ++it)
		delete *it;
	*/
}

void EntityManager::clearCharacters()
{
	// TODO delete entities as well

	/*
	for (auto it = gameEntities.begin(); it != gameEntities.end(); ++it)
		delete *it;

	for (auto it = currentLevelEntities.begin(); it != currentLevelEntities.end(); ++it)
		delete *it;
	*/
	gameEntities.clear();
	currentLevelEntities.clear();
	localPlayer = nullptr;
}

Player* EntityManager::localPlayer = nullptr;
std::vector<Entity*> EntityManager::gameEntities = std::vector<Entity*>();
std::vector<Entity*> EntityManager::currentLevelEntities = std::vector<Entity*>();
std::vector<Entity*> EntityManager::currentLevelTiles = std::vector<Entity*>();
QuadTree EntityManager::characters = QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 4086.0));
QuadTree EntityManager::tiles = QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 8192.0));

// TODO when entity is moved, it needs to be moved in the quadtree as well!