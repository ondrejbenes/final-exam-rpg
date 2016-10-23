#include "GamePhaseManager.h"

GamePhaseManager::GamePhaseManager(): _currentPhase(nullptr)
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

bool GamePhaseManager::changePhase(GamePhase* newPhase)
{
	_currentPhase = newPhase;

	return true;
}

GamePhaseManager* GamePhaseManager::instance = nullptr;