#include "Blackboard.h"


Blackboard::Blackboard(){

}



Blackboard::~Blackboard(){

}

std::vector<BlackboardCommand*> Blackboard::checkForMessages(Entity& entity)
{
	std::vector<BlackboardCommand*> messages;

	return messages;
}

std::vector<BlackboardCommand*> Blackboard::checkForMessages(Module& module)
{
	std::vector<BlackboardCommand*> messages;

	return messages;
}

void Blackboard::leaveMessage(Entity& entity, BlackboardCommand* message)
{
}

void Blackboard::leaveMessage(Module& module, BlackboardCommand* message)
{
}
