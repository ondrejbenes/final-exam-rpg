#include "Entity.h"

unsigned int Entity::nextId = 0;

Entity::Entity() : id(nextId++) {

}

Entity::~Entity(){

}

void Entity::update()
{
	for (auto comp : components)
		comp->update();
}
