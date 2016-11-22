#pragma once

#include "EntityComponent.h"

#include <SFML/Graphics.hpp>

#include <memory>

class GraphicsComponent : public EntityComponent
{
public:explicit GraphicsComponent(Entity& parent);

	void draw(std::shared_ptr<sf::RenderWindow> window) const;
	void update() override;

	void addSprite(const std::string& name, sf::Sprite sprite);

	void setActiveSprite(const std::string& name);
	sf::Sprite& getActiveSprite();

private:
	std::map<std::string, sf::Sprite> _sprites;
	std::string activeSprite;
};
