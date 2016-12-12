#pragma once

#include <vector>

#include "UiElement.h"

class UI
{
public:
	UI();

	void addElement(std::shared_ptr<UiElement> element);
	void removeElement(std::shared_ptr<UiElement> element);
	std::vector<std::shared_ptr<UiElement>> getElements() const { return _elements; }

	template<class T>
	std::shared_ptr<T> getElementByName(const std::string& name);

	std::shared_ptr<UiElement> getFocusedElement() const { return _focusedElement; }
	void setFocusedElement(std::shared_ptr<UiElement> element);
private:
	std::vector<std::shared_ptr<UiElement>> _elements;
	std::shared_ptr<UiElement> _focusedElement;
};

template <class T>
std::shared_ptr<T> UI::getElementByName(const std::string& name) 
{
	std::shared_ptr<T> ret = nullptr;

	for (auto element : _elements)
		if (element->getName() == name)
			ret = std::dynamic_pointer_cast<T>(element);

	return ret;
}
