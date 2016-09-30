#pragma once
#include "EntityComponent.h"

#include <SFML/System.hpp>

#include <vector>

// Forward declaration
class EntityComponent;
class EntityFactory;

// TODO consider making an animation component
enum MovementDirection { Down, DL, Left, UL, Up, UR, Right, RL, Still, Jump };

class Entity
{
	friend EntityFactory;
public:
	virtual ~Entity();
	const unsigned int id;

	virtual void update();

	template <typename T>
	T* getComponent();
protected:
	Entity();
	void addComponent(EntityComponent* component);
	void removeComponent(EntityComponent* component);
private:
	static unsigned int nextId;
	sf::Vector2f position;
	std::vector<EntityComponent*> components;
};

template <typename T>
T* Entity::getComponent()
{
	// TODO use cache so that we don't have to cycle during each frame

	auto typeToGet = typeid(T).name();
	for(auto it  = components.begin(); it != components.end(); ++it)
	{
		auto currentType = typeid(**it).name();
		if (strcmp(typeToGet, currentType) == 0)
			return const_cast<T*>(dynamic_cast<T*>(*it));
	}

	return nullptr;
}
