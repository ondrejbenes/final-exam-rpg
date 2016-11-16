#pragma once

#include "UiElement.h"
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class Button : public UiElement
{
public:
	Button(const std::string& text, const sf::Font& font, unsigned int fontSize = 16, const sf::Color& textColor = sf::Color::White, const sf::Color& backgroundColor = sf::Color(0, 0, 128, 192));

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
	void setPosition(const sf::Vector2f& pos) override;

	std::string getText() const { return _drawableText.getString(); }
	void setText(const std::string& text) { _drawableText.setString(text); }
	
private:
	sf::Text _drawableText;
	sf::RectangleShape _background;

	const static int padding = 10;
};
