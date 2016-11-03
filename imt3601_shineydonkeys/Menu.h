#pragma once

#include "GamePhase.h"

#include <SFML/Graphics/RenderWindow.hpp>

class Menu : public GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~Menu();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;
protected:
	Menu();

	void handleInput();
	void handleMouseReleased(const sf::Event& event);
};
