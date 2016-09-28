#include "GraphicsComponent.h"

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