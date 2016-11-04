#include "EntityManager.h"

std::shared_ptr<EntityManager> EntityManager::getInstance()
{
	if (_instance == nullptr)
		_instance = std::make_shared<EntityManager>(EntityManager());
	return _instance;
}

void EntityManager::add(Entity* entity)
{
	auto entityAsNodeData = reinterpret_cast<QuadTreeNodeData*>(entity);
	if (typeid(*entity) == typeid(Tile))
		_tiles.add(entityAsNodeData);
	else
		_characters.add(entityAsNodeData);
}

void EntityManager::remove(Entity* entity)
{
	auto entityAsNodeData = reinterpret_cast<QuadTreeNodeData*>(entity);
	if (typeid(*entity) == typeid(Tile))
		_tiles.remove(entityAsNodeData->getX(), entityAsNodeData->getY());
	else
		_characters.remove(entityAsNodeData->getX(), entityAsNodeData->getY());
}

void EntityManager::move(Entity* entity, const sf::Vector2f& previousPos)
{
	auto entityAsNodeData = reinterpret_cast<QuadTreeNodeData*>(entity);
	if (typeid(*entity) == typeid(Tile))
		_tiles.move(entityAsNodeData, previousPos.x, previousPos.y);
	else
		_characters.move(entityAsNodeData, previousPos.x, previousPos.y);
}

std::vector<Character*> EntityManager::getAllCharacters()
{
	std::vector<Character*> characters;

	auto boundary = QuadTreeBoundary(_characters.getBoundaryCpy());
	auto chars = _characters.getInInterval(&boundary);

	for (auto it = chars.begin(); it != chars.end(); ++it)
		characters.push_back(static_cast<Character*>(*it));

	return characters;
}

std::vector<Character*> EntityManager::getCharactersInInterval(QuadTreeBoundary& interval)
{
	std::vector<Character*> characters;

	auto charactersAsNodeData = _characters.getInInterval(&interval);

	for (auto it = charactersAsNodeData.begin(); it != charactersAsNodeData.end(); ++it)
		characters.push_back(static_cast<Character*>(*it));

	return characters;
}

Character* EntityManager::getCharacterAtPos(const sf::Vector2f& pos)
{
	auto character = _characters.get(pos.x, pos.y);
	Character* ret = nullptr;

	if (character != nullptr)
		ret = reinterpret_cast<Character*>(character);

	return ret;
}

std::vector<Tile*> EntityManager::getAllTiles()
{
	std::vector<Tile*> tiles;

	auto boundary = QuadTreeBoundary(_tiles.getBoundaryCpy());
	auto tilesAsNodeData = _tiles.getInInterval(&boundary);

	for (auto it = tilesAsNodeData.begin(); it != tilesAsNodeData.end(); ++it)
		tiles.push_back(reinterpret_cast<Tile*>(*it));

	return tiles;
}

std::vector<Tile*> EntityManager::getTilesInInterval(QuadTreeBoundary& interval)
{
	std::vector<Tile*> tiles;

	auto tilesAsNodeData = _tiles.getInInterval(&interval);

	for (auto it = tilesAsNodeData.begin(); it != tilesAsNodeData.end(); ++it)
		tiles.push_back(reinterpret_cast<Tile*>(*it));

	return tiles;
}

Tile* EntityManager::getTileAtPos(const sf::Vector2f& pos)
{
	auto tile = _tiles.get(pos.x, pos.y);
	Tile* ret = nullptr;

	if (tile != nullptr)
		ret = reinterpret_cast<Tile*>(tile);

	return ret;
}

// TODO remove magic constants
EntityManager::EntityManager() :
_localPlayer(nullptr),
_characters(QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 8192.0))),
_tiles(QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 8192.0)))
{

}

void EntityManager::clearCharacters()
{
	// TODO remove magic constants
	_characters = QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 8192.0));
	_localPlayer = nullptr;
}

void EntityManager::clearTiles()
{
	// TODO remove magic constants
	_tiles = QuadTree(new QuadTreeBoundary(0.0, 8192.0, 0, 8192.0));
}

std::shared_ptr<EntityManager> EntityManager::_instance = nullptr;