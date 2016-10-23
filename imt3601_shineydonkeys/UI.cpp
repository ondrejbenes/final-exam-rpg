#include "UI.h"


UI::UI(){

}



UI::~UI(){

}

void UI::addElement(sf::Drawable* element)
{
	_elements.push_back(element);
}

std::vector<sf::Drawable*> UI::getElements() const
{
	return _elements;
}
