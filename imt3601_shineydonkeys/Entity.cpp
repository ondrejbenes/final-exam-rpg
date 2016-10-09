#include "Entity.h"
#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include <sstream>

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

	getComponent<GraphicsComponent>()->getSprite().setPosition(position);
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
