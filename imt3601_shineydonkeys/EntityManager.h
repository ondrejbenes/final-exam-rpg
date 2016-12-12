#pragma once

#include "Entity.h"
#include "Player.h"
#include "QuadTree.h"

#include <memory>
#include <map>

// Forward declaration
class Tile;

/**
* \brief Singleton storage for all Entities (Characters and Tiles).
* Uses QuadTrees for storing.
*/
class EntityManager
{
public:
	/**
	* \brief Gets the unique instance of this class.
	* \return the unique instance
	*/
	static std::shared_ptr<EntityManager> getInstance();

	/* Returns the player */
	Player* getLocalPlayer() const { return _localPlayer; }

	/* Sets the local player*/
	void setLocalPlayer(Player* localPlayer) { _localPlayer = localPlayer; }
	
	/**
	 * \brief Adds the entity to the underlaying QuadTree.
	 * \param entity Entity to add
	 */
	void add(Entity* entity);

	/**
	* \brief Removes the entity to the underlaying QuadTree.
	* \param entity Entity to remove
	*/
	void remove(Entity* entity);

	/**
	 * \brief Moves an entity to a new position. 
	 * May result in reinsertion in the underlaying QuadTree.
	 * \param entity Entitny to move
	 * \param newPos New position
	 */
	void move(Entity* entity, const sf::Vector2f& newPos);

	/* Returns all Charaters in the underlaying QuadTree */
	std::vector<Character*> getAllCharacters();
	
	/**
	 * \brief Returns Charaters that are located in the interval (boundary)
	 * \param interval Boundary in which to search
	 * \return Vector of Charaters in the interval
	 */
	std::vector<Character*> getCharactersInInterval(const QuadTreeBoundary& interval);
	
	/* Returns Charater located at pos, or nullptr, if none was found */
	Character* getCharacterAtPos(const sf::Vector2f& pos);

	/* Returns Charater with the given Id */
	Character* getCharacterById(unsigned int id);

	/* Returns all Tiles in the underlaying QuadTree */
	std::vector<Tile*> getAllTiles();

	/**
	* \brief Returns Tiles that are located in the interval (boundary)
	* \param interval Boundary in which to search
	* \return Vector of Tiles in the interval
	*/
	std::vector<Tile*> getTilesInInterval(const QuadTreeBoundary& interval);
	
	/* Returns Tile located at pos, or nullptr, if none was found */
	Tile* getTileAtPos(const sf::Vector2f& pos);

	/* Returns Tile with the given Id */
	Tile* getTileById(unsigned int id);

	/* Celars the underlaying QuadTree, all Characters will be removed. */
	void clearCharacters();

	/* Celars the underlaying QuadTree, all Tiles will be removed. */
	void clearTiles();
protected:
	EntityManager();
private:
	static std::shared_ptr<EntityManager> _instance;

	Player* _localPlayer;
	QuadTree _characters;
	QuadTree _tiles;

	std::map<unsigned int, Entity*> _entityById;
};
