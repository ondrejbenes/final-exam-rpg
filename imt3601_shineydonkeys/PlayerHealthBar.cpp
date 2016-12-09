#include "PlayerHealthBar.h"
#include "EntityManager.h"

PlayerHealthBar::PlayerHealthBar(const sf::Vector2f& size) : 
_fullHpRect(size),
_currentHpRect(size),
_size(size)
{
	_fullHpRect.setFillColor(sf::Color::Red);
	_currentHpRect.setFillColor(sf::Color::Green);
}

void PlayerHealthBar::draw(std::shared_ptr<sf::RenderWindow> window) 
{
	auto view = window->getView();
	auto x = view.getCenter().x - view.getSize().x / 2;
	auto y = view.getCenter().y + view.getSize().y / 2 - _size.y;

	_fullHpRect.setPosition(x, y);

	auto stats = EntityManager::getInstance()->getLocalPlayer()->getStats();
	auto percentage = stats.current_hitpoints / float(stats.max_hitpoints);
	_currentHpRect.setSize(sf::Vector2f(_size.x * percentage, _size.y));
	_currentHpRect.setPosition(x, y);

	window->draw(_fullHpRect);
	window->draw(_currentHpRect);
}

sf::FloatRect PlayerHealthBar::getBounds() 
{
	return _fullHpRect.getGlobalBounds();
}
