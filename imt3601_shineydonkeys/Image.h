#pragma once

#include "UiElement.h"

#include <SFML/Graphics/Sprite.hpp>

class Image : public UiElement
{
public:
	explicit Image(sf::Texture* texture);

	void update() override;
	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
	void setPosition(const sf::Vector2f& pos) override;

	void changeTexture(sf::Texture* newTexture);
private:
	sf::Sprite _sprite;
};
