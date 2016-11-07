#include "Button.h"

Button::Button(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color& fillColor) :
_fontSize(fontSize)
{
	_drawableText = sf::Text(text, font, _fontSize);
	_drawableText.setFillColor(fillColor);
}

Button::~Button()
{

}

void Button::update()
{

}

void Button::draw(std::shared_ptr<sf::RenderWindow> window)
{
	window->draw(_drawableText);
}

sf::FloatRect Button::getBounds()
{
	return _drawableText.getGlobalBounds();
}

void Button::setPosition(const sf::Vector2f& pos)
{
	_drawableText.setPosition(pos);
}
