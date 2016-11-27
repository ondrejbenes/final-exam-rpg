#pragma once

#include "TextBox.h"
#include "Label.h"
#include "Button.h"

class UiElementFactory
{
public:
	Button* createButton(const wchar_t* configFile, const wchar_t* configSectionName, UiCallback onClick);
	Label* createLabel(const wchar_t* configFile, const wchar_t* configSectionName);
	TextBox* createTextBox(const wchar_t* configFile, const wchar_t* configSectionName);
	UiCallback* createChangeKeyButton(const std::wstring& moveDirection);
};