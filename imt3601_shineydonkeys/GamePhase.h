#pragma once

#include "UI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class GamePhaseFactory;

class GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~GamePhase();

	// virtual bool initialize() = 0;

	virtual void render(std::shared_ptr<sf::RenderWindow> window);
	virtual void update() = 0;

	UI& getUi();
protected:
	UI _ui;

	GamePhase();
};