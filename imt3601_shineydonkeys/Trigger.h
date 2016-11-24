#pragma once

#include "Entity.h"

#include <memory>
#include <functional>

#include <SFML/Graphics/Rect.hpp>

class Trigger
{
public:
	explicit Trigger(const sf::FloatRect& boundary);

	void registerEntity(Entity* entity);
	void unregisterEntity(Entity* entity);
	std::vector<Entity*>& getRegisteredEntities();
	
	const sf::FloatRect& getBoundary() const { return _boundary; }
	void setBoundary(const sf::FloatRect& boundary) { _boundary = boundary; }

	const std::shared_ptr<std::function<void(Entity* enteringEntity)>>& getOnTriggerEnter() const { return _onTriggerEnter; }
	void setOnTriggerEnter(const std::shared_ptr<std::function<void(Entity* enteringEntity)>>& onTriggerEnter) { _onTriggerEnter = onTriggerEnter; }
private:
	sf::FloatRect _boundary;
	std::shared_ptr<std::function<void(Entity* enteringEntity)>> _onTriggerEnter;
	std::vector <Entity*> _entitiesInTrigger;
};

