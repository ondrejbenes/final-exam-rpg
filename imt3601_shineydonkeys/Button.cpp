#include "Button.h"

Button::Button(const std::string& text, const sf::Font& font, unsigned int fontSize, const sf::Color& textColor, const sf::Color& backgroundColor)
{
	_drawableText = sf::Text(text, font, fontSize);
	_drawableText.setFillColor(textColor);

	_background.setSize(
		sf::Vector2f(
			_drawableText.getGlobalBounds().width + 2 * padding, 
			_drawableText.getGlobalBounds().height + 2 * padding));

	_background.setFillColor(backgroundColor);
}

void Button::draw(std::shared_ptr<sf::RenderWindow> window)
{
	window->draw(_background);
	window->draw(_drawableText);
}

sf::FloatRect Button::getBounds()
{
	return _background.getGlobalBounds();
}

void Button::setPosition(const sf::Vector2f& pos)
{
	UiElement::setPosition(pos);
	_background.setPosition(pos);
	_drawableText.setPosition(pos + sf::Vector2f(padding, padding / 2));
}
