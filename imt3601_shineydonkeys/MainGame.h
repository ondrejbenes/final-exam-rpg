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
	const static sf::Vector2f arenaTeleportPosition;

	virtual ~MainGame();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;

	void handleInput();
	void handleMovement();
	bool loadLevel(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);

protected:
	MainGame();

private:
	Level _currentLevel;
	sf::Clock _playerDeathTimer;
	sf::Clock _levelCompleteTimer;
	sf::Clock _teleportTimer;
	bool _playerDied;
	bool _levelComplete;
	bool _teleported;
	bool _teleportClockRestarted;
	bool _teleportFadedOut;

	void teleportToArena();

	void handlePlayerDeath();
	void handleLevelComplete();

	void drawHealthBar(std::shared_ptr<sf::RenderWindow> window);
	void loadControls();

	bool hasKeys();
};
