#pragma once
#include "Module.h"
#include "Level.h"
#include "GameLogic.h"
#include "GameState.h"
#include "GamePhase.h"
#include <string>

class Game : public Module
{

public:
	Game();
	virtual ~Game();
	static const std::string NAME;
	Level *m_Level;
	GameLogic *m_GameLogic;
	GameState *m_GameState;
	GamePhase *m_GamePhase;

	int loadLevel(std::string levelDefinition);

};
