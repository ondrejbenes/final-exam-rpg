#include "Entity.h"

unsigned int Entity::nextId = 0;

Entity::Entity() : id(nextId++) {

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

Entity::~Entity(){

}

void Entity::update()
{
	for (auto it = components.begin(); it != components.end(); ++it)
		(*it)->update();
}
