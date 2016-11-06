#include "CombatComponent.h"

CombatComponent::CombatComponent(Entity& parent) : EntityComponent(parent)
{

}

CombatComponent::~CombatComponent()
{

}

void CombatComponent::update()
{
	if (!_isInCombat)
		return;
	if (attackTimer.getElapsedTime().asMilliseconds() < _weapon.getAttackSpeedMs())
		return;
	auto damageModifier = (_weapon.getMaxDamage() - _weapon.getMinDamage());
	auto damage = _weapon.getMinDamage();
	_otherCombatComp->takeDamage(damage);
}

void CombatComponent::startCombat(Character* other)
{
	_isInCombat = true;
	_other = other;
	_otherCombatComp = _other->getComponent<CombatComponent>();
}

void CombatComponent::endCombat()
{
	_isInCombat = false;
	_other = nullptr;
	_otherCombatComp = nullptr;
}

void CombatComponent::takeDamage(const float damage)
{

}
