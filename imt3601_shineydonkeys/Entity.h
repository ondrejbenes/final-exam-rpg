#pragma once
#include "EntityComponent.h"
#include <SFML/System.hpp>
#include <vector>

// Forward declaration
class EntityComponent;

class Entity
{
public:
	Entity();
	virtual ~Entity();
	const unsigned int id;

	virtual void update();
private:
	static unsigned int nextId;
	sf::Vector2f position;
	std::vector<EntityComponent*> components;
};
