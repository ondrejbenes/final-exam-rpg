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
	std::vector<UiElement*> getElements() const { return _elements; }

	UiElement* getElementByName(const std::string& name);

	UiElement* getFocusedElement() const { return _focusedElement; }
	void setFocusedElement(UiElement* element);
private:
	std::vector<UiElement*> _elements;
	UiElement* _focusedElement;
};
