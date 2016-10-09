#pragma once

#include "Module.h"
#include "Level.h"
#include "GameLogic.h"
#include "GameState.h"
#include "GamePhase.h"

class Game : public Module
{
public:
	Level currentLevel;
	GameLogic gameLogic;
	GameState gameState;
	GamePhase* gamePhase;

	Game();
	virtual ~Game();
	bool initialize() override;
	void update() override;

	void quickSave();
	void quickLoad();
	bool loadLevel(std::string levelDefinition);
};