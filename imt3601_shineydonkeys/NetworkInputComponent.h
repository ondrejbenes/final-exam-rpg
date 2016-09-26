#pragma once
#include "EntityComponent.h"

class NetworkInputComponent : public EntityComponent
{

public:
	explicit NetworkInputComponent(Entity& parent);
	virtual ~NetworkInputComponent();

	void update() override;

};