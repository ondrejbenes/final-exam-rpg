#include "Entity.h"
#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

#include <sstream>

unsigned int Entity::nextId = 0;

Entity::Entity() : id(nextId++) {

}

void Entity::addComponent(EntityComponent* component)
{
	components[typeid(*component).name()] = component;
}

void Entity::update()
{
	for (auto it = components.begin(); it != components.end(); ++it)
		(*it).second->update();
}

sf::Vector2f Entity::getPosition()
{
	return position;
}

void Entity::setPosition(sf::Vector2f position)
{
	this->position = position;

	auto gc = getComponent<GraphicsComponent>();
	if(gc != nullptr)
	{
		gc->getActiveSprite().setPosition(position + gc->getSpriteOffset());		
	}

	auto pc = getComponent<PhysicsComponent>();
	if (pc != nullptr)
	{
		pc->getCollider().left = position.x + gc->getSpriteOffset().x;
		pc->getCollider().top = position.y + gc->getSpriteOffset().y;
	}
}

std::string Entity::toString() const
{
	std::string type = typeid(this).name();
	std::stringstream ss;
	ss << 
		"Type: " << typeid(*this).name() << ", " << 
		"Id: " << id << ", "
		"Pos: " << position.x << ";" << position.y;
	return ss.str();
}
