#pragma once

#include "EntityComponent.h"
#include "QuadTreeNodeData.h"

#include <vector>

#include <SFML/System.hpp>

// Forward declaration
class EntityComponent;
class EntityFactory;

class Entity : public QuadTreeNodeData
{
	friend EntityFactory;
public:
	static unsigned int nextId;
	const unsigned int id;

	virtual ~Entity();

	virtual void update();

	sf::Vector2f getPosition();
	void setPosition(sf::Vector2f position);

	template <typename T>
	T* getComponent();

	std::string toString() const;

	double getX() const override { return position.x; }
	double getY() const override { return position.y; }
	void setX(double x) override { position.x = x; }
	void setY(double y) override { position.y = y; }
	
	const std::string& getName() const { return name; }
	void setName(const std::string& name) { this->name = name; }
protected:
	sf::Vector2f position;
	std::vector<EntityComponent*> components;

	Entity();
	void addComponent(EntityComponent* component);
	void removeComponent(EntityComponent* component);
private:
	std::string name;
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
