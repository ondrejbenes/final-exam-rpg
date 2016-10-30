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

	auto direction = Still;
	if (velocity.x > 0)
		direction = Right;
	else if (velocity.x < 0)
		direction = Left;
	else if (velocity.y > 0)
		direction = Down;
	else if (velocity.y < 0)
		direction = Up;
	auto pc = parent.getComponent<AnimationComponent>();
	pc->animate(direction);
}

bool PhysicsComponent::hasCollision(const sf::Vector2f& newPosition)
{
	// TODO implement

	if (newPosition.x < 0 || newPosition.x > 8192.0 || newPosition.y < 0 || newPosition.y > 8192.0)
		return true;

	return false;
}

const sf::Vector2f PhysicsComponent::defaultVelocity = sf::Vector2f(300, 300);