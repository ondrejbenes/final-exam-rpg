#pragma once

#include "GamePhase.h"

class GamePhaseManager
{
public:
	~GamePhaseManager();

	static GamePhaseManager* getInstance();

	bool changePhase(GamePhase* newPhase);
	GamePhase* getCurrentPhase() const { return _currentPhase; };
private:
	GamePhaseManager();
	static GamePhaseManager* instance;

	GamePhase* _currentPhase;
};
