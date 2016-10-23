#pragma once

#include "Module.h"
#include "Level.h"
#include "GameLogic.h"
#include "GameState.h"

class Game : public Module
{
public:
	GameLogic gameLogic;
	GameState gameState;

	Game();
	virtual ~Game();
	bool initialize() override;
	void update() override;

	void quickSave();
	void quickLoad();
};