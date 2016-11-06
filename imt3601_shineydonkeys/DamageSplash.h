#pragma once

#include "UiElement.h"

#include <SFML/System/Time.hpp>
#include <SFML/System/Clock.hpp>

class DamageSplash : public UiElement
{
public:
	explicit DamageSplash(unsigned int damage, float x, float y);

	void update() override;

	~DamageSplash();
private:
	unsigned int _damage;
	sf::Time _lifetime = sf::seconds(2);
	sf::Clock _visibleForMs;
};
