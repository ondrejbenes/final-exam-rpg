#pragma once

#include "GamePhase.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Menu : public GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~Menu();

	void update() override;
	void render(sf::RenderWindow* window) override;

	void handleInput();
	void handleMouseReleased(const sf::Event& event);
protected:
	Menu();

};
