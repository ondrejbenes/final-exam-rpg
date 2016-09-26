#pragma once
#include "EntityComponent.h"
#include <SFML/Graphics.hpp>

class GraphicsComponent : public EntityComponent
{

public:
	explicit GraphicsComponent(Entity& parent);
	virtual ~GraphicsComponent();

	void draw(sf::RenderWindow& window);
	void update() override;

private:
	sf::Sprite sprite;

};
