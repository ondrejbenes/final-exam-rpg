#pragma once

#include "GamePhase.h"
#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>

struct Controls
{
	sf::Keyboard::Key up;
	sf::Keyboard::Key down;
	sf::Keyboard::Key left;
	sf::Keyboard::Key right;
};

class MainGame : public GamePhase
{
	friend GamePhaseFactory;
public:
	static Controls CONTROLS;

	virtual ~MainGame();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;

	void handleInput();
	void handleMovement();
	bool loadLevel(std::string levelDefinition);

protected:
	MainGame();

private:
	Level _currentLevel;

	void loadControls();
};
