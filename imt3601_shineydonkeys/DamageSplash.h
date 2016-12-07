#pragma once

#include "UiElement.h"

#include <memory>

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Graphics/Text.hpp>

class DamageSplash : public UiElement
{
public:
	DamageSplash(unsigned int damage, float x, float y);

	void draw(std::shared_ptr<sf::RenderWindow> window) override;
	sf::FloatRect getBounds() override;
private:
	unsigned int _damage;
	sf::Time _lifetime = sf::seconds(2);
	sf::Clock _visibleForMs;
	sf::Text _drawableText;
};
