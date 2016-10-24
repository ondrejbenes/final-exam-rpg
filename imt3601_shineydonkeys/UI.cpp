#include "UI.h"

UI::UI(){

}

UI::~UI(){

}

void UI::addElement(UiElement* element)
{
	_elements.push_back(element);
}

std::vector<UiElement*> UI::getElements() const
{
	return _elements;
}
