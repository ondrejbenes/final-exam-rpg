#pragma once

#include "UI.h"

#include <SFML/Graphics/RenderWindow.hpp>

class GamePhaseFactory;

class GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~GamePhase();

	// virtual bool initialize() = 0;

	virtual void render(sf::RenderWindow* window) = 0;
	virtual void update() = 0;

	UI& getUi();
protected:
	GamePhase();
	UI _ui;
};