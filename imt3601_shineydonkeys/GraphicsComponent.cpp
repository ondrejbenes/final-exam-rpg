#include "GraphicsComponent.h"
#include "Npc.h"
#include "CombatComponent.h"

GraphicsComponent::GraphicsComponent(Entity& parent) : EntityComponent(parent)
{

}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::draw(std::shared_ptr<sf::RenderWindow> window) const
{
	window->draw(_sprite);

	if(typeid(parent) == typeid(Npc))
	{
		if (parent.getComponent<CombatComponent>()->isInCombat())
		{
			auto fullHpRect = sf::RectangleShape(sf::Vector2f(_sprite.getGlobalBounds().width, 4));
			fullHpRect.setFillColor(sf::Color::Red);
			fullHpRect.setPosition(parent.getPosition().x, parent.getPosition().y - 10);

			auto parentAsChar = dynamic_cast<Character*>(&parent);
			auto stats = parentAsChar->getStats();
			auto percentage = stats->current_hitpoints / float(stats->max_hitpoints);
			auto currentHpRect = sf::RectangleShape(sf::Vector2f(_sprite.getGlobalBounds().width * percentage, 4));
			currentHpRect.setFillColor(sf::Color::Green);
			currentHpRect.setPosition(parent.getPosition().x, parent.getPosition().y - 10);

			window->draw(fullHpRect);
			window->draw(currentHpRect);
		}
	}
}

void GraphicsComponent::update()
{
	
}

void GraphicsComponent::setSprite(sf::Sprite& sprite)
{
	this->_sprite = sprite;
}

sf::Sprite& GraphicsComponent::getSprite()
{
	return _sprite;
}
