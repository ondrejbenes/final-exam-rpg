#pragma once

#include "AiState.h"
#include "CombatComponent.h"

class AiAttack : public AiState
{
public:
	AiAttack(AiComponent* component, sf::Vector2f center, float attackRadius);
	void update() override;

	static float MOVE_TOWARDS_PLAYER_RECALC_FREQ_MS;
private:
	sf::Vector2f _center;
	float _attackRadius;

	sf::Clock _moveTowardsPlayerClock;

	Character* _parentAsChar;
	CombatComponent* _parentCombatComp;
	CombatComponent* _otherCombatComp;
};

