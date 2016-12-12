#pragma once
#include "Entity.h"

// Forward declaration
class Entity;

class EntityComponent
{
public:
	explicit EntityComponent(Entity& parent);

	virtual void update() = 0;

	Entity& getParent() const{return parent;} 
protected:
	Entity& parent;
};
