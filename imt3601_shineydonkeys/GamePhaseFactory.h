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
private:
	UiElement* createMenuUiElement(const wchar_t* configFile, const wchar_t* configSectionName, std::function<void()> onClick);
};

