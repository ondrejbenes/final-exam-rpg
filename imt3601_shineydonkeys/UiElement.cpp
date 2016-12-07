#include "UiElement.h"

UiElement::UiElement() :
_name(""),
_onClick(nullptr), 
_onkeyPressed(nullptr),
_onTextEntered(nullptr) {}

void UiElement::setPosition(const sf::Vector2f& pos)
{
	this->_position.x = pos.x;
	this->_position.y = pos.y;
}
