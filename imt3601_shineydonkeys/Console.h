#pragma once

#include <vector>
#include <regex>
#include <functional>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>

/**
 * \brief Singleton responsible for parsing commands entered into its input and handeling them.
 * Can be drawn on a sf::RenderWindow.
 */
class Console
{
public:
	/**
	* \brief Gets the unique instance of this class.
	* \return the unique instance
	*/
	static std::shared_ptr<Console> getInstance();

	/**
	 * \brief Handle user input
	 * Can handle Enter key press (will attempt to handle input)
	 * and TextEntered (will append the entered text to input)
	 * \param event Event to handle
	 */
	void handleEvent(const sf::Event& event);

	/**
	 * \brief Draws itself to the supplied window.
	 * Will draw the last 10 lines on a black background with 50% oppacity.
	 * Will also draw current input line.
	 * \param window Window to draw on
	 */
	void draw(std::shared_ptr<sf::RenderWindow> window) const;

	/* Returns true if visible */
	bool isVisible() const { return _visible; }

	/* Sets the visibility */
	void setVisible(bool visible) { this->_visible = visible; }
protected:
	Console();
private:
	static std::shared_ptr<Console> instance;

	std::regex _regexMove;
	std::regex _regexZoom;
	std::vector<std::pair<std::regex, std::function<void()>>> _commands;

	unsigned int _characterSize = 12U;
	std::string _inputMarker = "> ";

	std::string _input;
	std::vector<std::string> _history;
	unsigned int _visibleMessagesCount = 10;
	bool _visible = false;

	/* will attempt to parse the input using regexes
	 * and if it succeedes, it will call the relevant function
	 */
	bool handleInput();

	void help();
	void list();
	void move();
	void zoom();
};
