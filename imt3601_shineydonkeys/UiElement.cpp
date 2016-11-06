#include "UiElement.h"

UiElement::UiElement(sf::Drawable* _graphics): _onClick(nullptr), _graphics(_graphics)
{
}

UiElement::~UiElement()
{
	delete _graphics;
}

void UiElement::update()
{

}
