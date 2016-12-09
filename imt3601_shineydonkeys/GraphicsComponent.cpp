#include "GraphicsComponent.h"
#include "Npc.h"
#include "CombatComponent.h"
#include "PhysicsComponent.h"
#include "Tile.h"

GraphicsComponent::GraphicsComponent(Entity& parent) :
	EntityComponent(parent)
{

}

void GraphicsComponent::draw(std::shared_ptr<sf::RenderWindow> window) const
{
	auto& sprite = _activeSprite->sprite;
	window->draw(sprite);

	if (DRAW_COLLIDERS)
	{
		if(typeid(parent) == typeid(Tile) && !dynamic_cast<Tile*>(&parent)->isBlocking())
			return;

		auto collider = parent.getComponent<PhysicsComponent>()->getCollider();
		auto colliderDrawing = sf::RectangleShape(sf::Vector2f(collider.width, collider.height));
		colliderDrawing.setPosition(sf::Vector2f(collider.left, collider.top));
		colliderDrawing.setFillColor(sf::Color(255, 0, 0, 64));

		window->draw(colliderDrawing);
	}

	if (typeid(parent) == typeid(Npc))
	{
		if (parent.getComponent<CombatComponent>()->isInCombat())
		{
			auto fullHpRect = sf::RectangleShape(sf::Vector2f(sprite.getGlobalBounds().width, 4));
			fullHpRect.setFillColor(sf::Color::Red);
			fullHpRect.setPosition(parent.getPosition().x, parent.getPosition().y - 10);

			auto parentAsChar = dynamic_cast<Character*>(&parent);
			auto stats = parentAsChar->getStats();
			auto percentage = stats.current_hitpoints / float(stats.max_hitpoints);
			auto currentHpRect = sf::RectangleShape(sf::Vector2f(sprite.getGlobalBounds().width * percentage, 4));
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

void GraphicsComponent::addSprite(const std::string& name, sf::Sprite sprite, const sf::Vector2u& cellsCount)
{
	auto it = find_if(begin(_sprites), end(_sprites), [name](const std::shared_ptr<SpriteWrapper>& other) {return other->name == name; });
	if(it == end(_sprites))
	{
		auto bounds = sprite.getGlobalBounds();
		auto xOffset = -(bounds.width / cellsCount.x / 2);
		auto yOffset = -(bounds.height / cellsCount.y / 2);
		auto spriteOffset = sf::Vector2f(xOffset, yOffset);

		_sprites.push_back(std::make_shared<SpriteWrapper>(name, sprite, cellsCount, spriteOffset));
	}
}

void GraphicsComponent::changeActiveSpriteTexRect(const sf::Vector2u& cell) 
{
	auto coord = cell.x + 1000 * cell.y;
	auto& rec = _activeSprite->texRecCashe[coord];
	_activeSprite->sprite.setTextureRect(rec);
}

void GraphicsComponent::setActiveSprite(const std::string& name)
{
	auto sprite = find_if(begin(_sprites), end(_sprites), [name](const std::shared_ptr<SpriteWrapper>& other) {return other->name == name; });
	if (sprite != end(_sprites))
		_activeSprite = *sprite;
}

sf::Sprite& GraphicsComponent::getActiveSprite() const 
{
	return _activeSprite->sprite;
}

const sf::Vector2f& GraphicsComponent::getSpriteOffset() const 
{
	return _activeSprite->offset;
}

GraphicsComponent::SpriteWrapper::SpriteWrapper(std::string name, sf::Sprite sprite, const sf::Vector2u& cellsCount, sf::Vector2f offset)
	: name(name),
	sprite(sprite),
	cellsCount(cellsCount),
	offset(offset) 
{
	auto spriteWidth = sprite.getTexture()->getSize().x / cellsCount.x;
	auto spriteHeight = sprite.getTexture()->getSize().y / cellsCount.y;

	for(auto y = 0; y < cellsCount.x; y++)
	{
		for (auto x = 0; x < cellsCount.y; x++)
		{
			auto coord = x + 1000 * y;

			texRecCashe[coord] = sf::IntRect(
				x * spriteWidth,
				y * spriteHeight,
				spriteWidth,
				spriteHeight);
		}
	}

	sprite.setTextureRect(texRecCashe[1001]);
}