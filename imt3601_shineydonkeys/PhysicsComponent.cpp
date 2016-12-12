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

	if (_velocity.x > 0)
		_direction = Right;
	else if (_velocity.x < 0)
		_direction = Left;
	else if (_velocity.y > 0)
		_direction = Down;
	else if (_velocity.y < 0)
		_direction = Up;

	auto radius = Tilemap::TILE_HEIGHT;
	auto boundary = QuadTreeBoundary(newPosition.x - radius, newPosition.x + radius, newPosition.y - radius, newPosition.y + radius);
	const auto& tilesToCheck = EntityManager::getInstance()->getTilesInInterval(boundary);

	radius *= 4;
	boundary = QuadTreeBoundary(newPosition.x - radius, newPosition.x + radius, newPosition.y - radius, newPosition.y + radius);
	const auto& charactersToCheck = EntityManager::getInstance()->getCharactersInInterval(boundary);

	checkTriggers(newPosition, tilesToCheck, charactersToCheck);

	if(hasCollision(newPosition, tilesToCheck, charactersToCheck))
	{
		_velocity = ZERO_VELOCITY;
		return;
	}
	EntityManager::getInstance()->move(&parent, newPosition);
	parent.setPosition(newPosition);
}

bool PhysicsComponent::hasCollision(const sf::Vector2f& newPosition, const std::vector<Tile*>& tilesToCheck, const std::vector<Character*>& charactersToCheck) const
{
	auto diff = newPosition - parent.getPosition();
	auto collCpy = sf::FloatRect(_collider);
	collCpy.left += diff.x;
	collCpy.top += diff.y;
			
	for (auto& tile : tilesToCheck)
	{
		auto pc = tile->getComponent<PhysicsComponent>();

		if (tile != nullptr && tile->isBlocking())
		{
			auto tileCollider = pc->getCollider();
			if (collCpy.intersects(tileCollider))
				return true;
		}
	}

	for (auto& character : charactersToCheck)
	{
		if (character->id != parent.id && character != nullptr)
		{
			auto pc = character->getComponent<PhysicsComponent>();

			auto collider = pc->getCollider();
			if (collCpy.intersects(collider))
				return true;
		}
	}

	if (newPosition.x < 0 || newPosition.x > Tilemap::MAP_WIDTH || newPosition.y < 0 || newPosition.y > Tilemap::MAP_HEIGHT)
		return true;

	return false;
}

void PhysicsComponent::checkTriggers(const sf::Vector2f& newPosition, const std::vector<Tile*>& tilesToCheck, const std::vector<Character*>& charactersToCheck) const
{
	for (auto& tile : tilesToCheck)
	{
		auto pc = tile->getComponent<PhysicsComponent>();

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
	}

	for (auto& character : charactersToCheck)
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
		}
	}
}

const std::string PhysicsComponent::MOVE_SPRITE_NAME = "move";

const sf::Vector2f PhysicsComponent::DEFAULT_PLAYER_VELOCITY = sf::Vector2f(225, 225);
const sf::Vector2f PhysicsComponent::DEFAULT_NPC_VELOCITY = sf::Vector2f(150, 150);
const sf::Vector2f PhysicsComponent::ZERO_VELOCITY = sf::Vector2f(0, 0);