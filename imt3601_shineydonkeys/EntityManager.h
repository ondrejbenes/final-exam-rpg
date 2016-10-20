#pragma once

#include "Entity.h"
#include "Player.h"
#include "QuadTree.h"

class Tile;

class EntityManager
{
public:
	static EntityManager* getInstance();

	Player* getLocalPlayer() const { return _localPlayer; }
	void setLocalPlayer(Player* localPlayer) { _localPlayer = localPlayer; }

	void add(Entity* entity);
	void remove(Entity* entity);
	void move(Entity* entity, const sf::Vector2f& newPos);

	std::vector<Character*> getAllCharacters();
	std::vector<Character*> getCharactersInInterval(QuadTreeBoundary& interval);
	Character* getCharacterAtPos(const sf::Vector2f& pos);

	std::vector<Tile*> getAllTiles();
	std::vector<Tile*> getTilesInInterval(QuadTreeBoundary& interval);
	Tile* getTileAtPos(const sf::Vector2f& pos);

	void clearCharacters();
	void clearTiles();
private:
	static EntityManager* _instance;

	Player* _localPlayer;
	QuadTree _characters;
	// TODO consider having tiles just in the tilemap class
	QuadTree _tiles;

	EntityManager();
	~EntityManager();
};
