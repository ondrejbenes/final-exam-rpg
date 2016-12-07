#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class UiElement;
typedef std::function<void(UiElement* source, const sf::Event& event)> UiCallback;

class UiElement
{
public:
	UiElement();

	virtual void draw(std::shared_ptr<sf::RenderWindow> window) = 0;
	virtual sf::FloatRect getBounds() = 0;
	
	std::string getName() const { return _name; }
	void setName(const std::string& name) { _name = name; }

	virtual void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition() const { return _position; }

	void setOnClick(UiCallback* callback) { _onClick = callback; }
	UiCallback* getOnClick() const { return _onClick; }

	void setOnKeyPressed(UiCallback* callback) { _onkeyPressed = callback; }
	UiCallback* getOnKeyPressed() const { return _onkeyPressed; }

	void setOnTextEntered(UiCallback* callback) { _onTextEntered = callback; }
	UiCallback* getOnTextEntered() const { return _onTextEntered; }

	virtual void setFocused(bool focused) { _focused = focused; }
	bool isFocused() const { return _focused; }
protected:
	std::string _name;
	sf::Vector2f _position;
	UiCallback* _onClick;
	UiCallback* _onkeyPressed;
	UiCallback* _onTextEntered;
	bool _focused = false;
};

