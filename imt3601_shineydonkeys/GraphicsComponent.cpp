#include "GraphicsComponent.h"
#include "Logger.h"
#include "PlayerInputComponent.h"

GraphicsComponent::GraphicsComponent(Entity& parent) : EntityComponent(parent)
{

}

GraphicsComponent::~GraphicsComponent()
{

}

void GraphicsComponent::draw(sf::RenderWindow* window) const
{
	window->draw(sprite);
}

void GraphicsComponent::update()
{
	
}

void GraphicsComponent::setSprite(sf::Sprite& sprite)
{
	this->sprite = sprite;
}

sf::Sprite& GraphicsComponent::getSprite()
{
	return sprite;
}
