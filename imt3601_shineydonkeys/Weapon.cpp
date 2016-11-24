#include "Weapon.h"

Weapon::Weapon(unsigned minDamage, unsigned maxDamage, unsigned attackSpeedMs) :
_minDamage(minDamage),
_maxDamage(maxDamage),
_attackSpeedMs(attackSpeedMs)
{

}

unsigned int Weapon::WEAPON_RANGE = 64;