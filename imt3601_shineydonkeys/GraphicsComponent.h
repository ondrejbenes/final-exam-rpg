#pragma once

#include "EntityComponent.h"

#include <SFML/Graphics.hpp>

#include <memory>

#define DRAW_COLLIDERS 0

class GraphicsComponent : public EntityComponent
{
public:explicit GraphicsComponent(Entity& parent);

	void draw(std::shared_ptr<sf::RenderWindow> window) const;
	void update() override;

	void addSprite(const std::string& name, sf::Sprite sprite, const sf::Vector2u& cellsCount = sf::Vector2u(1, 1));

	void changeActiveSpriteTexRect(const sf::Vector2u& cell);
	void setActiveSprite(const std::string& name);
	sf::Sprite& getActiveSprite() const;

	const sf::Vector2f& getSpriteOffset() const;
private:
	class SpriteWrapper
	{
	public:
		SpriteWrapper(std::string name, sf::Sprite sprite, const sf::Vector2u& cellsCount, sf::Vector2f offset);

		std::string name;
		sf::Sprite sprite;
		sf::Vector2u cellsCount;
		sf::Vector2f offset;

		std::map<int, sf::IntRect> texRecCashe;

		bool operator==(const SpriteWrapper& other) const { return name == other.name; }
		bool operator==(const std::string& other) const { return name == other; }
	};

	std::vector<std::shared_ptr<SpriteWrapper>> _sprites;
	std::shared_ptr<SpriteWrapper> _activeSprite;
};
