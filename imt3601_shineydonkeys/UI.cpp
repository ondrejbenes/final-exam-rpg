#include "UI.h"

UI::UI(){

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

std::vector<UiElement*> UI::getElements() const
{
	return _elements;
}
