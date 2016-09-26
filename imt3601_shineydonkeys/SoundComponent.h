#pragma once
#include "EntityComponent.h"

class SoundComponent : public EntityComponent
{

public:
	explicit SoundComponent(Entity& parent);
	virtual ~SoundComponent();

	void update() override;

};