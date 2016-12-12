#pragma once

#include "EntityComponent.h"
#include "Character.h"

class CombatComponent : public EntityComponent
{
public:
	static const std::string COMBAT_SPRITE_NAME;

	explicit CombatComponent(Entity& parent);

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
	sf::Clock attackTimer;
};

