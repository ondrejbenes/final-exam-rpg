#pragma once

#include "EntityComponent.h"
#include "QuadTreeNodeData.h"

#include <map>
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
	
	const std::string& getName() const { return _name; }
	void setName(const std::string& name) { this->_name = name; }
protected:
	sf::Vector2f position;
	std::map<const char*, EntityComponent*> components;

	Entity();
	void addComponent(EntityComponent* component);
private:
	std::string _name;
};

template <typename T>
T* Entity::getComponent()
{
	return const_cast<T*>(dynamic_cast<T*>(components[typeid(T).name()]));
}
