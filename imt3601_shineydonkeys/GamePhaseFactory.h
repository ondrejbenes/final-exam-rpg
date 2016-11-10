#pragma once

#include "MainGame.h"
#include "Menu.h"
#include "UiElement.h"

class GamePhaseFactory
{
public:
	MainGame* createMainGame();
	Menu* createMainMenu();
	Menu* createStartMultiPlayerGame();
	Menu* createJoinMultiPlayerGame();
	Menu* createOptions();
private:
	UiElement* createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, uiCallback onClick);
};

