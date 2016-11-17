#include "Entity.h"
#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include <sstream>
#include "PhysicsComponent.h"

unsigned int Entity::nextId = 0;

Entity::Entity() : id(nextId++) {

}

Entity::~Entity() {

}

void Entity::addComponent(EntityComponent* component)
{
	// TODO - allow only one component of a single type?
	components.push_back(component);
}

void Entity::removeComponent(EntityComponent* component)
{
	for (auto it = components.begin(); it != components.end(); ++it)
	{
		if (*it == component)
			components.erase(it);
	}
}

void Entity::update()
{
	for (auto it = components.begin(); it != components.end(); ++it)
		(*it)->update();
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
		gc->getSprite().setPosition(position);

	auto pc = getComponent<PhysicsComponent>();
	if (pc != nullptr)
	{
		pc->getCollider().left = position.x;
		pc->getCollider().top = position.y;
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
