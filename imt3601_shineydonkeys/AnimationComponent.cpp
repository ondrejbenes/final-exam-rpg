#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Logger.h"

#include <sstream>

#include <SFML/Graphics/Rect.hpp>

AnimationComponent::AnimationComponent(Entity& parent) : 
EntityComponent(parent),
spriteSheetCell(sf::Vector2i(1, 0)),
_sprite (parent.getComponent<GraphicsComponent>()->getSprite())
{
	_spriteWidth = _sprite.getTexture()->getSize().x / 4;
	_spriteHeight = _sprite.getTexture()->getSize().y / 4;

	_sprite.setTextureRect(
		sf::IntRect(
			spriteSheetCell.x * _spriteWidth,
			spriteSheetCell.y * _spriteHeight,
			_spriteWidth,
			_spriteHeight));
}

AnimationComponent::~AnimationComponent()
{

}

void AnimationComponent::update()
{
	auto velocity = parent.getComponent<PhysicsComponent>()->getVelocity();
	if (velocity == PhysicsComponent::ZERO_VELOCITY)
	{
		if (spriteSheetCell.x == 0 || spriteSheetCell.x == 2)
			return;
	}
	else 
	{
		auto direction = Down;
		if (velocity.x > 0)
			direction = Right;
		else if (velocity.x < 0)
			direction = Left;
		else if (velocity.y > 0)
			direction = Down;
		else if (velocity.y < 0)
			direction = Up;

		if (spriteSheetCell.y != direction)
			spriteSheetCell.x = 0;
		spriteSheetCell.y = direction;
	}

	if (_animationTimer.getElapsedTime() > ANIMATION_PERIOD_MS)
	{
		spriteSheetCell.x++;
		_animationTimer.restart();
	}

	if (spriteSheetCell.x == 4)
		spriteSheetCell.x = 0;
	
	// TODO make a cache
	_sprite.setTextureRect(
		sf::IntRect(
			spriteSheetCell.x * _spriteWidth, 
			spriteSheetCell.y * _spriteHeight, 
			_spriteWidth, 
			_spriteHeight));
	
	_sprite.setPosition(parent.getPosition());
}

sf::Time AnimationComponent::ANIMATION_PERIOD_MS = sf::milliseconds(100);