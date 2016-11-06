#pragma once

#include "EntityComponent.h"
#include "Character.h"
#include "Weapon.h"

class CombatComponent : public EntityComponent
{
public:
	explicit CombatComponent(Entity& parent);
	virtual ~CombatComponent();

	void update() override;

	bool isInCombat() const { return _inCombat; }

	Character* getOther() const { return _other; }

	void startCombat(Character* other);
	void endCombat();
	void takeDamage(const unsigned int damage);
private:
	bool _inCombat;
	Character* _other;
	CombatComponent* _otherCombatComp;
	Weapon* _weapon;
	sf::Clock attackTimer;
};

