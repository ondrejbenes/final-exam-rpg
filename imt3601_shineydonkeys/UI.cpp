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

UiElement* UI::getElementByName(const std::string& name)
{
	for(auto* element : _elements)
		if (element->getName() == name)
			return element;

	return nullptr;
}

void UI::setFocusedElement(UiElement* element)
{
	if(find(begin(_elements), end(_elements), element) == end(_elements) && element != nullptr)
		return;

	if (_focusedElement != nullptr)
		_focusedElement->setFocused(false);

	_focusedElement = element;

	if (element != nullptr)
		element->setFocused(true);
}
