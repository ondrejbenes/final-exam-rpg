#pragma once

#include "Item.h"

class Weapon : public Item
{
public:
	static unsigned int WEAPON_RANGE;

	Weapon(unsigned damage, unsigned maxDamage, unsigned attackSpeedMs);
	
	unsigned getMinDamage() const { return _minDamage;	}
	void setMinDamage(unsigned damage) { this->_minDamage = damage; }


	unsigned getMaxDamage() const {	return _maxDamage; }
	void setMaxDamage(unsigned maxDamage) {	_maxDamage = maxDamage; }

	unsigned getAttackSpeedMs() const { return _attackSpeedMs; }
	void setAttackSpeedMs(unsigned attackSpeedMs) { this->_attackSpeedMs = attackSpeedMs; }

private:
	unsigned int _minDamage;
	unsigned int _maxDamage;
	unsigned int _attackSpeedMs;
};

