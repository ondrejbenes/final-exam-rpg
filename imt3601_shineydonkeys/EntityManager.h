#pragma once

#include <vector>

#include "Entity.h"
#include "Player.h"
#include "QuadTree.h"

class EntityManager
{
public:
	static Player* localPlayer;

	// entities that live the entire game - mostly players
	static std::vector<Entity*> gameEntities;

	// level entities, most notably NPCs
	static std::vector<Entity*> currentLevelEntities;

	// level tiles
	static std::vector<Entity*> currentLevelTiles;

	static QuadTree characters;
	static QuadTree tiles;

	EntityManager();
	~EntityManager();

	static void clearCharacters();
};
