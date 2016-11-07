#pragma once

#include "GamePhase.h"

class Options : public GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~Options();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;
protected:
	Options();

	void handleInput();
	void handleMouseReleased(const sf::Event& event);
	void handleKeyPressed(const sf::Event& event);
};