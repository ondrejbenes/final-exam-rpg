#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/Event.hpp>

class UiElement
{
public:
	UiElement();
	virtual ~UiElement();

	virtual void update() = 0;
	virtual void draw(std::shared_ptr<sf::RenderWindow> window) = 0;
	virtual sf::FloatRect getBounds() = 0;

	virtual void setPosition(const sf::Vector2f& pos);
	const sf::Vector2f& getPosition() const { return _position; }

	void setOnClick(std::function<void(UiElement* source, const sf::Event& event)>* callback) { _onClick = callback; }
	std::function<void(UiElement* source, const sf::Event& event)>* getOnClick() const { return _onClick; }

	void setOnKeyPressed(std::function<void(UiElement* source, const sf::Event& event)>* callback) { _onkeyPressed = callback; }
	std::function<void(UiElement* source, const sf::Event& event)>* getOnKeyPressed() const { return _onkeyPressed; }

	void setOnTextEntered(std::function<void(UiElement* source, const sf::Event& event)>* callback) { _onTextEntered = callback; }
	std::function<void(UiElement* source, const sf::Event& event)>* getOnTextEntered() const { return _onTextEntered; }
protected:
	sf::Vector2f _position;
	std::function<void(UiElement* source, const sf::Event& event)>* _onClick;
	std::function<void(UiElement* source, const sf::Event& event)>* _onkeyPressed;
	std::function<void(UiElement* source, const sf::Event& event)>* _onTextEntered;
};
