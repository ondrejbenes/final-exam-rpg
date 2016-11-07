#include "UI.h"

UI::UI() : 
_focusedElement(nullptr)
{
}

UI::~UI(){

}

void UI::addElement(UiElement* element)
{
	_elements.push_back(element);
}

void UI::removeElement(UiElement* element)
{
	_elements.erase(remove(begin(_elements), end(_elements), element), _elements.end());
}

void UI::setFocusedElement(UiElement* element)
{
	// TODO should do check if element belongs to this UI?
	_focusedElement = element;
}
