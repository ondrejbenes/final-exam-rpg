#pragma once

#include <vector>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Font.hpp>
#include <regex>
#include <functional>

//TODO refactor singleton
class Console
{
public:
	virtual ~Console();
	static Console* getInstance();
	bool handleInput();
	void handleEvent(const sf::Event& event);
	void draw(sf::RenderWindow* window) const;

	bool isVisible() const { return visible; }
	void setVisible(bool visible) { this->visible = visible; }
private:
	Console();
	static Console* instance;
	static int VISIBLE_MESSAGES_COUNT;

	std::regex _regexMove;
	std::regex _regexZoom;
	std::vector<std::pair<std::regex, std::function<void()>>> _commands;

	sf::Font font;
	unsigned int characterSize = 12U;
	std::string inputMarker = "> ";

	std::vector<std::string> history;
	std::string input;
	bool visible;

	void help();
	void list();
	void move();
	void zoom();
};
