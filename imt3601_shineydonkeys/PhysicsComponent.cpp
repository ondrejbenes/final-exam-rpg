#include "PhysicsComponent.h"
#include "AnimationComponent.h"
#include "EntityManager.h"

PhysicsComponent::PhysicsComponent(Entity& parent) : 
EntityComponent(parent) ,
velocity(0, 0)
{

}

PhysicsComponent::~PhysicsComponent()
{

}

void PhysicsComponent::update()
{
	move();
}

void PhysicsComponent::setVelocity(sf::Vector2f velocity)
{
	this->velocity = velocity;
}

sf::Vector2f PhysicsComponent::getVelocity()
{
	return velocity;
}

void PhysicsComponent::move()
{
	auto ellapsed = sinceLastMove.restart().asSeconds();
	auto newPosition = parent.getPosition() + velocity * ellapsed;

	if(hasCollision(newPosition))
	{
		velocity = sf::Vector2f(0, 0);
		return;
	}

	EntityManager::getInstance()->move(&parent, newPosition);
}

bool PhysicsComponent::hasCollision(const sf::Vector2f& newPosition) const
{
	// TODO implement

	if (newPosition.x < 0 || newPosition.x > 8192.0 || newPosition.y < 0 || newPosition.y > 8192.0)
		return true;

	return false;
}

const sf::Vector2f PhysicsComponent::defaultVelocity = sf::Vector2f(200, 200);

const sf::Vector2f PhysicsComponent::ZERO_VELOCITY = sf::Vector2f(0, 0);