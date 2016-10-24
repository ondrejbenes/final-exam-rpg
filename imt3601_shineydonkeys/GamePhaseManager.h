#pragma once

#include "GamePhase.h"

#include <stack>

class GamePhaseManager
{
public:
	~GamePhaseManager();

	static GamePhaseManager* getInstance();

	void pushPhase(GamePhase* phase);
	void popPhase();
	GamePhase* getCurrentPhase() const;
private:
	GamePhaseManager();
	static GamePhaseManager* instance;

	std::stack<GamePhase*> _phases;
};
