#include "Blackboard.h"
#include "Module.h"
#include <iterator>
#include "Logger.h"

Blackboard::Blackboard()
{
	LOG_D("Creating Blackboard");
}

Blackboard::~Blackboard()
{

}

Blackboard* Blackboard::getInstance()
{
	if (instance == nullptr)
		instance = new Blackboard;
	return instance;
}

std::vector<std::function<void(Module*)>> Blackboard::getCallbacks(ModuleType moduleType)
{
	std::vector<std::function<void(Module*)>> callbacks;
	copy(allCallbacks[moduleType].begin(), allCallbacks[moduleType].end(), std::back_inserter(callbacks));
	allCallbacks[moduleType].clear();

	return callbacks;
}

void Blackboard::leaveCallback(ModuleType moduleType, std::function<void(Module*)> callback)
{
	allCallbacks[moduleType].push_back(callback);
}

Blackboard* Blackboard::instance = nullptr;
