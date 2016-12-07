#pragma once

#include "UiElement.h"
#include <SFML/Graphics/RectangleShape.hpp>

class Inventory : public UiElement
{
public:
	const static unsigned int width = 310;
	const static unsigned int height = 210;
	const static unsigned int padding = 10;
	const static unsigned int iconWidth = 90;
	const static unsigned int iconHeight = 90;
	const static unsigned int iconsPerRow = 3;
	const static unsigned int maxItems = 6;

	Inventory();

	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;

	unsigned getHighlightedItem() const { return _highlightedItem; }
	void setHighlightedItem(unsigned highlightedItem) { _highlightedItem = highlightedItem; }
private:
	sf::RectangleShape _background;
	unsigned int _highlightedItem;
};

