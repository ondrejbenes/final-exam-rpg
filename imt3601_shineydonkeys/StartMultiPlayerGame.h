#pragma once

#include "GamePhase.h"

class StartMultiPlayerGame : public GamePhase
{
	friend GamePhaseFactory;
public:
	~StartMultiPlayerGame();

	void update() override;
	void render(sf::RenderWindow* window) override;
protected:
	StartMultiPlayerGame();

	void handleInput();
	void handleMouseReleased(const sf::Event& event);
};

