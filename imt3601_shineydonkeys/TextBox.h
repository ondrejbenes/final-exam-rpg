#pragma once

#include "UiElement.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class TextBox : public UiElement
{
public:
	TextBox(const sf::Font& font, const std::string& text = "", unsigned int fontSize = 16, const sf::Color& textColor = sf::Color::White);

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
	void setPosition(const sf::Vector2f& pos) override;

	void setFocused(bool focused) override;

	std::string getText();
	void setText(const std::string& text);

	void resetOutlineSize();
private:
	bool showingVerticalLine = false;
	sf::Text _drawableText;
	sf::RectangleShape _outline;
	sf::Clock _verticalLineBlinkTimer;
	const static sf::Time blinkFreq;
	const static int padding = 6;
};

