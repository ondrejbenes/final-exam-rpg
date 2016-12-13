#pragma once

#include "UI.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>

class GamePhaseFactory;

class GamePhase
{
	friend GamePhaseFactory;
public:
	virtual void render(std::shared_ptr<sf::RenderWindow> window);
	virtual void update() = 0;

	UI& getUi();

	virtual ~GamePhase() {} ;
protected:
	UI _ui;

	GamePhase(){}

	void handleGuiEvent(const sf::Event& event);

	void handleMouseReleased(const sf::Event& event);
	void handleTextEntered(const sf::Event& event);
	void handleKeyPressed(const sf::Event& event);
};