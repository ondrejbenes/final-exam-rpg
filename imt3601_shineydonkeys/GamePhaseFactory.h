#pragma once

#include "MainGame.h"
#include "Menu.h"
#include "UiElementFactory.h"

class GamePhaseFactory
{
public:
	MainGame* createMainGame();
	Menu* createMainMenu();
	Menu* createStartMultiPlayerGame();
	Menu* createJoinMultiPlayerGame();
	Menu* createOptions();
private:
	UiElementFactory _uiElementFactory;
};

