#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class UiElement;
typedef std::function<void(UiElement* source, const sf::Event& event)> uiCallback;

class UiElement
{
public:
	UiElement();
	virtual ~UiElement();

	virtual void update() = 0;
	virtual void draw(std::shared_ptr<sf::RenderWindow> window) = 0;
	virtual sf::FloatRect getBounds() = 0;
	
	std::string getName() const { return _name; }
	void setName(const std::string& name) { _name = name; }

	virtual void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition() const { return _position; }

	void setOnClick(uiCallback* callback) { _onClick = callback; }
	uiCallback* getOnClick() const { return _onClick; }

	void setOnKeyPressed(uiCallback* callback) { _onkeyPressed = callback; }
	uiCallback* getOnKeyPressed() const { return _onkeyPressed; }

	void setOnTextEntered(uiCallback* callback) { _onTextEntered = callback; }
	uiCallback* getOnTextEntered() const { return _onTextEntered; }
protected:
	std::string _name;
	sf::Vector2f _position;
	uiCallback* _onClick;
	uiCallback* _onkeyPressed;
	uiCallback* _onTextEntered;
};

