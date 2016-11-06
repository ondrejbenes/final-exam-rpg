#pragma once

#include "AiState.h"
#include "CombatComponent.h"

class AiAttack : public AiState
{
public:
	AiAttack(AiComponent* component, sf::Vector2f center, float attackRadius);
	~AiAttack();
	void update() override;
private:
	// TODO duplicity with AiPatrol
	sf::Vector2f _center;
	float _attackRadius;

	CombatComponent* _parentCombatComp;
	CombatComponent* _otherCombatComp;
};

