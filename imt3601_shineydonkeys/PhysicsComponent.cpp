#include "PhysicsComponent.h"
#include "AnimationComponent.h"

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
	if(hasCollision()) 
		return;

	parent.setPosition(sf::Vector2f(parent.getPosition() + velocity));
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

bool PhysicsComponent::hasCollision()
{
	// TODO implement
	return false;
}

const sf::Vector2f PhysicsComponent::defaultVelocity = sf::Vector2f(10, 10);