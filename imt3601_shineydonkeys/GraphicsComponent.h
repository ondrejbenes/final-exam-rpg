#pragma once
#include "EntityComponent.h"
#include <SFML/Graphics.hpp>

class GraphicsComponent : public EntityComponent
{

public:
	sf::Vector2i* spriteSheetCell;

	explicit GraphicsComponent(Entity& parent);
	virtual ~GraphicsComponent();

	void draw(sf::RenderWindow* window) const;
	void update() override;

private:
	sf::Sprite spriteSheet;
	sf::Texture playerTexture;

};
