#pragma once
#include "Entity.h"

// Forward declaration
class Entity;

class EntityComponent
{

public:
	explicit EntityComponent(Entity& parent);
	virtual ~EntityComponent();

	virtual void update();

protected:
	Entity& parent;

};
