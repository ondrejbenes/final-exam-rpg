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

	bool isVisible() const { return _visible; }
	void setVisible(bool visible) { this->_visible = visible; }
private:
	Console();
	static Console* instance;
	unsigned int _visibleMessagesCount;

	std::regex _regexMove;
	std::regex _regexZoom;
	std::vector<std::pair<std::regex, std::function<void()>>> _commands;

	// TODO Res loader
	sf::Font font;
	unsigned int _characterSize = 12U;
	std::string _inputMarker = "> ";

	std::vector<std::string> _history;
	std::string _input;
	bool _visible;

	void help();
	void list();
	void move();
	void zoom();
};
