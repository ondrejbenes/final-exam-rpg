#include "DamageSplash.h"

#include "ResourceLoader.h"
#include "EntityManager.h"
#include "GamePhaseManager.h"
#include "Logger.h"

DamageSplash::DamageSplash(unsigned damage, float x, float y) :
UiElement(nullptr),
_damage(damage)
{
	// TODO change to different font?
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	auto fontSize = 16;

	auto damageText = new sf::Text(std::to_string(_damage), font, fontSize);
	damageText->setPosition(x, y);
	damageText->setFillColor(sf::Color::White);
	damageText->setStyle(sf::Text::Style::Bold);

	_graphics = damageText;
}

void DamageSplash::update()
{
	auto text = dynamic_cast<sf::Text*>(_graphics);
	auto elapsedTime = _visibleForMs.getElapsedTime().asSeconds();
	auto newY = text->getPosition().y - elapsedTime * 10;
	text->setPosition(text->getPosition().x, newY);
	text->setFillColor(sf::Color(255, 255, 255, 255 - 255 * (elapsedTime / _lifetime.asSeconds())));
	
	if (elapsedTime > _lifetime.asSeconds())
	{
		GamePhaseManager::getInstance()->getCurrentPhase()->getUi().removeElement(this);
	}
}


DamageSplash::~DamageSplash()
{

}
