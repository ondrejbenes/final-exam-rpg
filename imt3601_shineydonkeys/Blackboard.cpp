#include "Blackboard.h"
#include "Module.h"
#include <iterator>

std::shared_ptr<Blackboard> Blackboard::getInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<Blackboard>(Blackboard());
	return instance;
}

void Blackboard::leaveCallback(ModuleType moduleType, std::function<void(Module*)> callback)
{
	_allCallbacks[moduleType].push_back(callback);
}

std::vector<std::function<void(Module*)>> Blackboard::getCallbacks(ModuleType moduleType)
{
	std::vector<std::function<void(Module*)>> callbacks;
	copy(_allCallbacks[moduleType].begin(), _allCallbacks[moduleType].end(), std::back_inserter(callbacks));
	_allCallbacks[moduleType].clear();

	return callbacks;
}

void Blackboard::pushEvent(const sf::Event& e)
{
	_windowEvents.push(e);
}

bool Blackboard::pollEvent(sf::Event& e)
{
	if(_windowEvents.empty())
		return false;
	e = _windowEvents.front();
	_windowEvents.pop();
	return true;
}

std::shared_ptr<Blackboard> Blackboard::instance = nullptr;
