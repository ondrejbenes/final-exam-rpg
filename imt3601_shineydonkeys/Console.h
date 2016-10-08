#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>

//TODO refactor singleton
class Console
{
public:
	virtual ~Console();

	static Console* getInstance();
	bool handleInput();
	void draw(sf::RenderWindow* window) const;

	std::string getInput() { return input; }
	void setInput(std::string input) { this->input = input; }

	bool isVisible() const { return visible; }
	void setVisible(bool visible) { this->visible = visible; }
private:
	Console();
	static Console* instance;

	sf::Font font;
	unsigned int characterSize = 12U;
	std::string inputMarker = "> ";

	std::vector<std::string> history;
	std::string input;
	bool visible;
};
