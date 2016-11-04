#include "GamePhaseManager.h"

GamePhaseManager::~GamePhaseManager()
{
	while(popPhase())
	{
		// Pop the phases, so that they will be deleted
	}
}

std::shared_ptr<GamePhaseManager> GamePhaseManager::getInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<GamePhaseManager>(GamePhaseManager());
	return instance;
}

void GamePhaseManager::pushPhase(GamePhase* phase)
{
	_phases.push(phase);
}

bool GamePhaseManager::popPhase()
{
	if (_phases.empty())
		return false;

	auto current = _phases.top();
	delete current;
	_phases.pop();

	return true;
}

GamePhase* GamePhaseManager::getCurrentPhase() const
{
	return _phases.top();
}

std::shared_ptr<GamePhaseManager> GamePhaseManager::instance = nullptr;