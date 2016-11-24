#include "Trigger.h"

Trigger::Trigger(const sf::FloatRect& boundary) : _boundary(boundary)
{

}

void Trigger::registerEntity(Entity* entity) 
{
	_entitiesInTrigger.push_back(entity);
}

void Trigger::unregisterEntity(Entity* entity) 
{
	auto result = find(begin(_entitiesInTrigger), end(_entitiesInTrigger), entity);

	if (result != end(_entitiesInTrigger))
		_entitiesInTrigger.erase(result);
}

std::vector<Entity*>& Trigger::getRegisteredEntities() 
{
	return _entitiesInTrigger;
}
