#pragma once

#include "UiElement.h"
#include "Image.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Minimap : public UiElement
{
public:
	explicit Minimap(sf::Texture* texture);

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
private:
	sf::Sprite _image;
	sf::RectangleShape _visibleArea;
	sf::CircleShape _playerPosition;

	float _imageWidth;
	float _imageHeight;
};
