#pragma once

#include "GamePhase.h"

class Options : public GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~Options();

	void update() override;
	void render(sf::RenderWindow* window) override;
protected:
	Options();

	void handleInput();
	void handleMouseReleased(const sf::Event& event);
};