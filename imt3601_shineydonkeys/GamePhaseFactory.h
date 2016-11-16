#pragma once

#include "MainGame.h"
#include "Menu.h"
#include "UiElement.h"
#include "Button.h"
#include "Label.h"
#include "TextBox.h"

class GamePhaseFactory
{
public:
	MainGame* createMainGame();
	Menu* createMainMenu();
	Menu* createStartMultiPlayerGame();
	Menu* createJoinMultiPlayerGame();
	Menu* createOptions();
private:
	Button* createButton(const wchar_t* configFile, const wchar_t* configSectionName, UiCallback onClick);
	Label* createLabel(const wchar_t* configFile, const wchar_t* configSectionName);
	TextBox* createTextBox(const wchar_t* configFile, const wchar_t* configSectionName);
	UiCallback* createChangeKeyButton(const std::wstring& moveDirection);
};

