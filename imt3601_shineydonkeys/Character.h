#pragma once
#include "Entity.h"
#include <memory>
#include "Item.h"
#include "Weapon.h"

struct CharacterStats
{
	unsigned int max_hitpoints;
	unsigned int current_hitpoints;
};

class EntityFactory;

class Character : public Entity
{
	friend EntityFactory;
public:
	CharacterStats& getStats() { return _stats; }

	std::vector<std::shared_ptr<Item>>& getInventory() { return _inventory; }
	
	std::shared_ptr<Weapon> getEquipedWeapon() const { return _equipedWeapon; }
	void setEquipedWeapon(const std::shared_ptr<Weapon>& equipedWeapon) { _equipedWeapon = equipedWeapon; }
protected:
	explicit Character(CharacterStats stats);
private:
	CharacterStats _stats;
	std::vector<std::shared_ptr<Item>> _inventory;
	std::shared_ptr<Weapon> _equipedWeapon;
};