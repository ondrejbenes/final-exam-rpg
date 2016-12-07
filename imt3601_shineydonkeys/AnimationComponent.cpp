#include "AnimationComponent.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"

#include <SFML/Graphics/Rect.hpp>
#include "CombatComponent.h"

AnimationComponent::AnimationComponent(Entity& parent) : 
EntityComponent(parent),
spriteSheetCell(sf::Vector2i(1, 0))
{

}

void AnimationComponent::update()
{
	auto cc = parent.getComponent<CombatComponent>();
	if (cc->isInCombat())
		doCombatAnimation();
	else
		doMoveAnimation();
}

void AnimationComponent::doCombatAnimation() 
{
	checkIncrementCellX(COMBAT_ANIMATION_PERIOD_MS);

	checkChangeCellY();

	setTextureRect();
}

void AnimationComponent::doMoveAnimation() 
{
	auto velocity = parent.getComponent<PhysicsComponent>()->getVelocity();

	if (velocity == PhysicsComponent::ZERO_VELOCITY)
	{
		if (spriteSheetCell.x == 0/* || spriteSheetCell.x == 2*/)
			return;
	}
	else
	{
		checkChangeCellY();
	}
	checkIncrementCellX(MOVE_ANIMATION_PERIOD_MS);

	setTextureRect();
}

void AnimationComponent::checkIncrementCellX(const sf::Time& animationPeriod)
{
	if (_animationTimer.getElapsedTime() > animationPeriod)
	{
		spriteSheetCell.x = spriteSheetCell.x == 3 ? 0 : ++spriteSheetCell.x;
		_animationTimer.restart();
	}
}

void AnimationComponent::checkChangeCellY() {
	auto direction = parent.getComponent<PhysicsComponent>()->getDirection();

	if (spriteSheetCell.y != direction)
	{
		spriteSheetCell.x = 0;
		spriteSheetCell.y = direction;
	}
}

void AnimationComponent::setTextureRect() {
	// TODO make a cache
	auto& sprite = parent.getComponent<GraphicsComponent>()->getActiveSprite();
	auto spriteWidth = sprite.getTexture()->getSize().x / 4;
	auto spriteHeight = sprite.getTexture()->getSize().y / 4;
	sprite.setTextureRect(
		sf::IntRect(
			spriteSheetCell.x * spriteWidth,
			spriteSheetCell.y * spriteHeight,
			spriteWidth,
			spriteHeight));
}

sf::Time AnimationComponent::MOVE_ANIMATION_PERIOD_MS = sf::milliseconds(100);
sf::Time AnimationComponent::COMBAT_ANIMATION_PERIOD_MS = sf::milliseconds(250);