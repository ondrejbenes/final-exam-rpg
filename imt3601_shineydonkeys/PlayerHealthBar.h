#pragma once
#include "UiElement.h"
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include "Character.h"

class PlayerHealthBar :
	public UiElement
{
public:
	explicit PlayerHealthBar(const sf::Vector2f& size);

	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
private:
	sf::RectangleShape _fullHpRect;
	sf::RectangleShape _currentHpRect;

	sf::Vector2f _size;
};
