#include "UI.h"

UI::UI() : 
_focusedElement(nullptr)
{

}

void UI::addElement(std::shared_ptr<UiElement> element)
{
	_elements.push_back(element);
}

void UI::removeElement(std::shared_ptr<UiElement> element)
{
	_elements.erase(remove(begin(_elements), end(_elements), element), _elements.end());
}

void UI::setFocusedElement(std::shared_ptr<UiElement> element)
{
	if(find(begin(_elements), end(_elements), element) == end(_elements) && element != nullptr)
		return;

	if (_focusedElement != nullptr)
		_focusedElement->setFocused(false);

	_focusedElement = element;

	if (element != nullptr)
		element->setFocused(true);
}
