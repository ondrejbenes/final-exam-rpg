#pragma once
#include "EntityComponent.h"

class AiComponent : public EntityComponent
{

public:
	explicit AiComponent(Entity& parent);
	virtual ~AiComponent();

	void update() override;
private:
	// TODO remove when we have smarter AI
	int framesInOneDirection = 0;
};