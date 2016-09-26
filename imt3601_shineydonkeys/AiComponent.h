#pragma once
#include "EntityComponent.h"

class AiComponent : public EntityComponent
{

public:
	explicit AiComponent(Entity& parent);
	virtual ~AiComponent();

	void update() override;

};