#include "GamePhaseManager.h"

GamePhaseManager::GamePhaseManager()
{

}

GamePhaseManager::~GamePhaseManager()
{
	// delete _currentPhase;
}

GamePhaseManager* GamePhaseManager::getInstance()
{
	if (instance == nullptr)
		instance = new GamePhaseManager;
	return instance;
}

void GamePhaseManager::pushPhase(GamePhase* phase)
{
	_phases.push(phase);
}

void GamePhaseManager::popPhase()
{
	auto current = _phases.top();
	delete current;
	_phases.pop();
}

GamePhase* GamePhaseManager::getCurrentPhase() const
{
	return _phases.top();
}


GamePhaseManager* GamePhaseManager::instance = nullptr;
