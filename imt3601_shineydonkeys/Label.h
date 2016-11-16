#pragma once

#include "UiElement.h"

#include <SFML/Graphics/Text.hpp>

class Label : public UiElement
{
public:
	Label(const std::string& text, const sf::Font& font, unsigned int fontSize = 16, const sf::Color& fillColor = sf::Color::White);
	
	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
	void setPosition(const sf::Vector2f& pos) override;
	
	const std::string& getText() const { return _drawableText.getString(); }
	void setText(const std::string& text) { _drawableText.setString(text); }
private:
	sf::Text _drawableText;
};