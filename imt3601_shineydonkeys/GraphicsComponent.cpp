#include "GraphicsComponent.h"

GraphicsComponent::GraphicsComponent(Entity& parent) : EntityComponent(parent)
{

}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::draw(std::shared_ptr<sf::RenderWindow> window) const
{
	window->draw(_sprite);
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
