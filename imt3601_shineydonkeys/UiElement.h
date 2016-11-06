#pragma once
#include <SFML/Graphics/Drawable.hpp>
#include <functional>

class UiElement
{
public:
	explicit UiElement(sf::Drawable* _graphics);
	~UiElement();

	virtual void update();

	void setOnClick(std::function<void()>* callback) { _onClick = callback; }
	std::function<void()>* getOnClick() const { return _onClick; }

	sf::Drawable* getGraphics() const { return _graphics; }
protected:
	sf::Drawable* _graphics;
private:
	std::function<void()>* _onClick;
};
