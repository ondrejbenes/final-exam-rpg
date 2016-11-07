#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Drawable.hpp>
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

	void setOnClick(std::function<void()>* callback) { _onClick = callback; }
	std::function<void()>* getOnClick() const { return _onClick; }

	void setOnTextEntered(std::function<void(const sf::Event& text)>* callback) { _onTextEntered = callback; }
	std::function<void(const sf::Event& text)>* getOnTextEntered() const { return _onTextEntered; }
protected:
	sf::Vector2f _position;
	std::function<void()>* _onClick;
	std::function<void(const sf::Event& text)>* _onTextEntered;
};
