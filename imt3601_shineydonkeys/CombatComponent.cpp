#include "CombatComponent.h"
#include "Logger.h"
#include <sstream>

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
}

void CombatComponent::endCombat()
{
	_inCombat = false;
	_other = nullptr;
	_otherCombatComp = nullptr;
}

void CombatComponent::takeDamage(const float damage)
{
	std::stringstream ss;
	auto parent = getParent();
	ss << "Entity " << parent.id << " takes " << damage << " damage";
	LOG_D(ss.str());
}
