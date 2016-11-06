#include "Character.h"

Character::Character(CharacterStats stats)
{
	_stats = new CharacterStats(stats);
}

Character::~Character()
{

}