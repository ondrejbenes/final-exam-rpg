#pragma once

#include "UiElement.h"

#include <vector>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class ChatBoard : public UiElement
{
public:
	const static unsigned int fontSize = 12;
	const static unsigned int width = 400;
	const static unsigned int height = 200;
	const static char inputMarker = '>';

	ChatBoard();

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;

	std::string& getInput() { return _input; }

	void addMessage(const std::string& author, const std::string& message);
private:
	sf::RectangleShape _background;
	std::vector<std::string> _messages;
	std::string _input;
};
