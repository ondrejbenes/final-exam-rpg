#pragma once

#include "MainGame.h"
#include "Menu.h"
#include "StartMultiPlayerGame.h"
#include "Options.h"

class GamePhaseFactory
{
public:
	GamePhaseFactory();
	~GamePhaseFactory();

	MainGame* createMainGame();
	Menu* createMainMenu();
	StartMultiPlayerGame* createStartMultiPlayerGame();
	Options* createOptions();
private:
	UiElement* createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, std::function<void()> onClick);
};

