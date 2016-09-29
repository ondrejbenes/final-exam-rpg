#pragma once

#include "BlackboardCommand.h"

#include <vector>

class Module;
class Entity;

class Blackboard
{

public:
	Blackboard();
	virtual ~Blackboard();

	std::vector<BlackboardCommand*> checkForMessages(Entity& entity);
	std::vector<BlackboardCommand*> checkForMessages(Module& module);
	void leaveMessage(Entity& entity, BlackboardCommand* message);
	void leaveMessage(Module& module, BlackboardCommand* message);

};
