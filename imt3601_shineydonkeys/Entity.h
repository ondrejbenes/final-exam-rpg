#pragma once
#include "EntityComponent.h"

#include <SFML/System.hpp>

#include <vector>

// Forward declaration
class EntityComponent;
class EntityFactory;

class Entity
{
	friend EntityFactory;
public:
	virtual ~Entity();
	const unsigned int id;

	virtual void update();
protected:
	Entity();
private:
	static unsigned int nextId;
	sf::Vector2f position;
	std::vector<EntityComponent*> components;
};
