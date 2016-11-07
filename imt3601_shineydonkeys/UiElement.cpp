#include "UiElement.h"

UiElement::UiElement() : 
_onClick(nullptr), 
_onTextEntered(nullptr)
{

}

UiElement::~UiElement()
{
	
}

void UiElement::setPosition(const sf::Vector2f& pos)
{
	this->_position.x = pos.x;
	this->_position.y = pos.y;
}
