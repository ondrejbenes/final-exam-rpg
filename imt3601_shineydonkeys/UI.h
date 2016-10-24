#pragma once

#include <vector>

#include "UiElement.h"

class UI
{

public:
	UI();
	virtual ~UI();

	void addElement(UiElement* element);
	std::vector<UiElement*> getElements() const;
private:
	std::vector<UiElement*> _elements;
};
