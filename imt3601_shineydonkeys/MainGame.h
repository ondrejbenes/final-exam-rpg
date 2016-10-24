#pragma once

#include "GamePhase.h"
#include "Level.h"

class MainGame : public GamePhase
{
	friend GamePhaseFactory;
public:
	virtual ~MainGame();

	void update() override;
	void render(sf::RenderWindow* window) override;

	void handleInput();
	void handleMovement();
	bool loadLevel(std::string levelDefinition);

protected:
	MainGame();

private:
	Level _currentLevel;
};
