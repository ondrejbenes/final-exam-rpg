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

	void startCombat(Character* other);
	void endCombat();
	void takeDamage(const float damage);
private:
	bool _isInCombat;
	Character* _other;
	CombatComponent* _otherCombatComp;
	Weapon& _weapon;
	sf::Clock attackTimer;
};

