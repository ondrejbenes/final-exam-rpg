#pragma once

#include "UiElement.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>

class Button : public UiElement
{
public:
	Button(const std::string& text, const sf::Font& font, unsigned int fontSize = 16, const sf::Color& fillColor = sf::Color::White);
	~Button();

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
	void setPosition(const sf::Vector2f& pos) override;

	/*
	std::string getText() const { return _text; }
	void setText(const std::string& text) { _text = text; }

	sf::Font getFont() const { return _font; }
	void setFont(sf::Font& font) { _font = font; }

	unsigned getFontSize() const { return _fontSize; }
	void setFontSize(const unsigned fontSize) { _fontSize = fontSize; }
	*/

private:
	sf::Text _drawableText;
	std::string _text;
	unsigned int _fontSize;
};
