#pragma once

#include "TextBox.h"
#include "Label.h"
#include "Button.h"

class UiElementFactory
{
public:
	std::shared_ptr<Button> createButton(const wchar_t* configFile, const wchar_t* configSectionName, UiCallback onClick);
	std::shared_ptr<Label> createLabel(const wchar_t* configFile, const wchar_t* configSectionName);
	std::shared_ptr<TextBox> createTextBox(const wchar_t* configFile, const wchar_t* configSectionName);
	UiCallback* createChangeKeyButton(const std::wstring& moveDirection);
};