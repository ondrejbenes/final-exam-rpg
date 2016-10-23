#pragma once
#include "MainGame.h"
#include "Menu.h"

class GamePhaseFactory
{
public:
	GamePhaseFactory();
	~GamePhaseFactory();

	MainGame* createMainGame();
	Menu* createMainMenu();
};

