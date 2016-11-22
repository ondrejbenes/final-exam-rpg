#include "PhysicsComponent.h"
#include "AnimationComponent.h"
#include "EntityManager.h"
#include "Network.h"
#include "Blackboard.h"
#include "MainGame.h"
#include "GamePhaseFactory.h"
#include "Tile.h"
#include "GraphicsComponent.h"
#include "Logger.h"

PhysicsComponent::PhysicsComponent(Entity& parent, bool _static) : 
EntityComponent(parent) ,
_velocity(0, 0),
_static(_static)
{

}

void PhysicsComponent::update()
{
	if(!_static)
		move();
}

void PhysicsComponent::setVelocity(sf::Vector2f velocity)
{
	this->_velocity = velocity;

	auto id = parent.id;
	Blackboard::getInstance()->leaveCallback(
		NETWORK,
		[id, velocity](Module* target)
		{
			PacketFactory factory;
			auto packet = factory.createVelocityChange(id, velocity);
			dynamic_cast<Network*>(target)->broadcast(packet);
		}
	);
}

sf::Vector2f PhysicsComponent::getVelocity()
{
	return _velocity;
}

void PhysicsComponent::move()
{
	auto ellapsed = _sinceLastMove.restart().asSeconds();
	auto newPosition = parent.getPosition() + _velocity * ellapsed;

	if(hasCollision(newPosition))
	{
		_velocity = sf::Vector2f(0, 0);
		return;
	}
	EntityManager::getInstance()->move(&parent, newPosition);
	parent.setPosition(newPosition);
}

bool PhysicsComponent::hasCollision(const sf::Vector2f& newPosition) const
{
	// TODO better way to look for tiles (and characters) to check

	auto radius = 32;
	auto boundary = QuadTreeBoundary(newPosition.x - radius, newPosition.x + radius, newPosition.y - radius, newPosition.y + radius);

	auto tilesToCheck = EntityManager::getInstance()->getTilesInInterval(boundary);

	/*int tileX = int(newPosition.x) / Tilemap::TILE_WIDTH * Tilemap::TILE_WIDTH;
	int tileY = int(newPosition.y) / Tilemap::TILE_HEIGHT * Tilemap::TILE_HEIGHT;

	auto tile = EntityManager::getInstance()->getTileAtPos(sf::Vector2f(tileX, tileY));*/
	
	for (auto tile : tilesToCheck)
	{
		if (tile != nullptr && tile->isBlocking())
		{
			auto tileCollider = tile->getComponent<PhysicsComponent>()->getCollider();
			if (_collider.intersects(tileCollider))
				return true;
		}
	}

	auto charactersToCheck = EntityManager::getInstance()->getCharactersInInterval(boundary);

	for (auto character : charactersToCheck)
	{
		if (character->id != parent.id && character != nullptr)
		{
			auto collider = character->getComponent<PhysicsComponent>()->getCollider();
			if (_collider.intersects(collider))
				return true;
		}
	}

	if (newPosition.x < 0 || newPosition.x > Tilemap::MAP_WIDTH || newPosition.y < 0 || newPosition.y > Tilemap::MAP_HEIGHT)
		return true;

	return false;
}

const std::string PhysicsComponent::MOVE_SPRITE_NAME = "move";

const sf::Vector2f PhysicsComponent::defaultVelocity = sf::Vector2f(200, 200);
const sf::Vector2f PhysicsComponent::ZERO_VELOCITY = sf::Vector2f(0, 0);