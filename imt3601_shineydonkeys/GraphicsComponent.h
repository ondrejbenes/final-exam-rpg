#pragma once
#include "EntityComponent.h"
#include <SFML/Graphics.hpp>

class GraphicsComponent : public EntityComponent
{

public:

	explicit GraphicsComponent(Entity& parent);
	virtual ~GraphicsComponent();

	void draw(sf::RenderWindow* window) const;
	void update() override;

	void setSprite(sf::Sprite& sprite);
	sf::Sprite& getSprite();

private:
	sf::Sprite sprite;
};
