#include "Label.h"

Label::Label(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color& fillColor)
{
	_drawableText = sf::Text(text, font, fontSize);
	_drawableText.setFillColor(fillColor);
}

void Label::draw(std::shared_ptr<sf::RenderWindow> window)
{
	window->draw(_drawableText);
}

sf::FloatRect Label::getBounds() 
{
	return _drawableText.getGlobalBounds();
}

void Label::setPosition(const sf::Vector2f& pos) 
{
	UiElement::setPosition(pos);
	_drawableText.setPosition(pos);
}
