#include "CombatComponent.h"
#include "Logger.h"
#include <sstream>
#include "EntityManager.h"

CombatComponent::CombatComponent(Entity& parent) :
EntityComponent(parent),
_inCombat(false), 
_other(nullptr), 
_otherCombatComp(nullptr)
{
	auto& children = parent.getChildren();
	for (auto it = begin(children); it != end(children); ++it)
		if (typeid(**it) == typeid(Weapon))
			_weapon = dynamic_cast<Weapon*>(*it);
}

CombatComponent::~CombatComponent()
{

}

void CombatComponent::update()
{
	if (!_inCombat)
		return;
	if (attackTimer.getElapsedTime().asMilliseconds() < _weapon->getAttackSpeedMs())
		return;
	attackTimer.restart();
	auto damageModifier = rand() % (_weapon->getMaxDamage() - _weapon->getMinDamage());
	auto damage = _weapon->getMinDamage() + damageModifier;
	_otherCombatComp->takeDamage(damage);
}

void CombatComponent::startCombat(Character* other)
{
	_inCombat = true;
	_other = other;
	_otherCombatComp = _other->getComponent<CombatComponent>();


	//TODO move to animetionComponent and fix

	//auto texture = new sf::Texture;
	//if (!texture->loadFromFile("Resources/Images/Player3.png"))
	//	LOG_E("Error: could not load player image");
	//sf::Sprite sprite;
	//sprite.setTexture(*texture);

	//auto gc = parent.getComponent<GraphicsComponent>();
	//gc->setSprite(sprite);

}

void CombatComponent::endCombat()
{
	_inCombat = false;
	_other = nullptr;
	_otherCombatComp = nullptr;
}

void CombatComponent::takeDamage(const unsigned int damage)
{
	std::stringstream ss;
	auto parent = getParent();

	auto stats = dynamic_cast<Character*>(&getParent())->getStats(); 
	stats->current_hitpoints = (damage > stats->current_hitpoints) ? 0U : (stats->current_hitpoints - damage);

	ss << "Entity " << parent.id << " takes " << damage << " damage, " << stats->current_hitpoints << " left";
	LOG_D(ss.str());

	if (stats->current_hitpoints == 0)
	{
		// TODO stays alive one frame longer?
		LOG_D("DEAD");
		if (_otherCombatComp != nullptr && _otherCombatComp->getOther()->id == parent.id)
			_otherCombatComp->endCombat();
		endCombat();
		EntityManager::getInstance()->remove(&parent);
	}
}
