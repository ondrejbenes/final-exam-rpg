#pragma once
#include <vector>
#include "Entity.h"

class EntityManager
{
public:
	static Entity* localPlayer;

	// entities that live the entire game - mostly players
	static std::vector<Entity*> gameEntities;

	// level entities, most notably NPCs
	static std::vector<Entity*> currentLevelEntities;

	// level entities, most notably NPCs
	static std::vector<Entity*> currentLevelTiles;

	EntityManager();
	~EntityManager();
};