#pragma once

#include <vector>

#include <SFML/Graphics/Drawable.hpp>

class UI
{

public:
	UI();
	virtual ~UI();

	void addElement(sf::Drawable* element);
	std::vector<sf::Drawable*> getElements() const;
private:
	std::vector<sf::Drawable*> _elements;
};
