#pragma once

#include <vector>

#include "UiElement.h"

class UI
{

public:
	UI();
	virtual ~UI();

	void addElement(UiElement* element);
	void removeElement(UiElement* element);
	std::vector<UiElement*> getElements() const;
private:
	std::vector<UiElement*> _elements;
};
