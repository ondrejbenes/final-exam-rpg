#pragma once
#include "EntityComponent.h"
#include <SFML/Graphics.hpp>
#include <memory>

class GraphicsComponent : public EntityComponent
{
public:
	explicit GraphicsComponent(Entity& parent);
	virtual ~GraphicsComponent();

	void draw(std::shared_ptr<sf::RenderWindow> window) const;
	void update() override;

	void setSprite(sf::Sprite& sprite);
	sf::Sprite& getSprite();

private:
	sf::Sprite _sprite;
};
