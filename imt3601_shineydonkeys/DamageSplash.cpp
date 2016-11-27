#include "DamageSplash.h"
#include "ResourceLoader.h"
#include "GamePhaseManager.h"
#include "Logger.h"

DamageSplash::DamageSplash(unsigned int damage, float x, float y) :
_damage(damage)
{
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = 16;

	_drawableText = sf::Text(std::to_string(_damage), font, fontSize);
	_drawableText.setPosition(x, y);
	_drawableText.setFillColor(sf::Color::White);
	_drawableText.setStyle(sf::Text::Style::Bold);
}

void DamageSplash::update()
{
	auto elapsedTime = _visibleForMs.getElapsedTime().asSeconds();
	auto newY = _drawableText.getPosition().y - elapsedTime * 10;
	_drawableText.setPosition(_drawableText.getPosition().x, newY);
	_drawableText.setFillColor(sf::Color(255, 255, 255, 255 - 255 * (elapsedTime / _lifetime.asSeconds())));
	
	if (elapsedTime > _lifetime.asSeconds())
		GamePhaseManager::getInstance()->getCurrentPhase()->getUi().removeElement(this);
}

void DamageSplash::draw(std::shared_ptr<sf::RenderWindow> window)
{
	window->draw(_drawableText);
}

sf::FloatRect DamageSplash::getBounds()
{
	return _drawableText.getGlobalBounds();
}