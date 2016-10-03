#pragma once
#include "EntityComponent.h"

class PlayerInputComponent : public EntityComponent
{
public:
	explicit PlayerInputComponent(Entity& parent);
	virtual ~PlayerInputComponent();

	void update() override;
};