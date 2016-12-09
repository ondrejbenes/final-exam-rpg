#pragma once

#include "GamePhase.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Menu : public GamePhase
{
	friend GamePhaseFactory;
public:
	void update() override;
protected:
	Menu(){}

	void handleInput();
};
