#include "PhysicsComponent.h"
#include "AnimationComponent.h"
#include "EntityManager.h"
#include "Network.h"
#include "Blackboard.h"
#include "MainGame.h"
#include "Tile.h"
#include "GraphicsComponent.h"
#include "Tilemap.h"
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

	// TODO a bit strange to handle enter outside and leave inside
	std::vector<Entity*> toUnregister;
	for (auto trigger : _triggers)
	{
		for (auto entity : trigger->getRegisteredEntities())
			if (!entity->getComponent<PhysicsComponent>()->getCollider().intersects(trigger->getBoundary()))
				toUnregister.push_back(entity);

		for (auto entity : toUnregister)
			trigger->unregisterEntity(entity);
	}

}

void PhysicsComponent::setVelocity(sf::Vector2f velocity)
{
	this->_velocity = velocity;

	if(!Network::isServer())
		return;

	auto position = parent.getPosition();
	auto id = parent.id;

	Blackboard::getInstance()->leaveCallback(
		NETWORK,
		[id, velocity, position](Module* target)
		{
			PacketFactory factory;
			auto network = dynamic_cast<Network*>(target);

			auto positionPacket = factory.createPositionChange(id, position);
			network->broadcast(positionPacket);

			auto velocityPacket = factory.createVelocityChange(id, velocity);
			network->broadcast(velocityPacket);
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

	auto diff = newPosition - parent.getPosition();
	auto collCpy = sf::FloatRect(_collider);
	collCpy.left += diff.x;
	collCpy.top += diff.y;

	auto radius = 32;
	auto boundary = QuadTreeBoundary(newPosition.x - radius, newPosition.x + radius, newPosition.y - radius, newPosition.y + radius);

	auto tilesToCheck = EntityManager::getInstance()->getTilesInInterval(boundary);

	/*int tileX = int(newPosition.x) / Tilemap::TILE_WIDTH * Tilemap::TILE_WIDTH;
	int tileY = int(newPosition.y) / Tilemap::TILE_HEIGHT * Tilemap::TILE_HEIGHT;

	auto tile = EntityManager::getInstance()->getTileAtPos(sf::Vector2f(tileX, tileY));*/
	
	for (auto tile : tilesToCheck)
	{
		auto pc = tile->getComponent<PhysicsComponent>();

		// TODO check colliders somewhere else
		auto& triggers = pc->getTriggers();
		for (auto trigger : triggers)
		{
			if (_collider.intersects(trigger->getBoundary()))
			{
				auto& registeredEntities = trigger->getRegisteredEntities();
				auto result = find(begin(registeredEntities), end(registeredEntities), &parent);
				if(result == end(registeredEntities))
				{					
					trigger->registerEntity(&parent);
					auto callback = *trigger->getOnTriggerEnter();
					callback(&parent);
				}
			}
		}

		if (tile != nullptr && tile->isBlocking())
		{
			auto tileCollider = pc->getCollider();
			if (collCpy.intersects(tileCollider))
				return true;
		}
	}

	// TODO duplicity
	radius = 128;
	boundary = QuadTreeBoundary(newPosition.x - radius, newPosition.x + radius, newPosition.y - radius, newPosition.y + radius);
	auto charactersToCheck = EntityManager::getInstance()->getCharactersInInterval(boundary);

	for (auto character : charactersToCheck)
	{
		if (character->id != parent.id && character != nullptr)
		{
			auto pc = character->getComponent<PhysicsComponent>();

			auto& triggers = pc->getTriggers();
			for (auto trigger : triggers)
			{
				if (_collider.intersects(trigger->getBoundary()))
				{
					auto& registeredEntities = trigger->getRegisteredEntities();
					auto result = find(begin(registeredEntities), end(registeredEntities), &parent);
					if (result == end(registeredEntities))
					{
						trigger->registerEntity(&parent);
						auto callback = *trigger->getOnTriggerEnter();
						callback(&parent);
					}
				}
			}

			auto collider = pc->getCollider();
			if (collCpy.intersects(collider))
				return true;
		}
	}

	if (newPosition.x < 0 || newPosition.x > Tilemap::MAP_WIDTH || newPosition.y < 0 || newPosition.y > Tilemap::MAP_HEIGHT)
		return true;

	return false;
}

const std::string PhysicsComponent::MOVE_SPRITE_NAME = "move";

const sf::Vector2f PhysicsComponent::DEFAULT_PLAYER_VELOCITY = sf::Vector2f(225, 225);
const sf::Vector2f PhysicsComponent::DEFAULT_NPC_VELOCITY = sf::Vector2f(150, 150);
const sf::Vector2f PhysicsComponent::ZERO_VELOCITY = sf::Vector2f(0, 0);