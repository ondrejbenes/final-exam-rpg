#pragma once

#include "GamePhase.h"
#include "Level.h"
#include <SFML/Window/Keyboard.hpp>
#include <memory>
#include "Tile.h"

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

	static bool donkeyTextShown;

	const static sf::Vector2f arenaTunnelEntrance;
	const static sf::Vector2f arenaTunnelExit;

	const static sf::Vector2f bronzeKeyUnlockTile;
	const static sf::Vector2f bronzeKeyGateTile;

	const static sf::Vector2f silverKeyUnlockTile;
	const static sf::Vector2f silverKeyGateTile;

	const static sf::Vector2f goldKeyUnlockTile;
	const static sf::Vector2f goldKeyGateTile;

	virtual ~MainGame();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;

	void handlePlayerDeath();
	bool loadLevel(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);

protected:
	MainGame();

private:
	Level _currentLevel;
	bool _levelComplete;

	bool _escapePressed;

	std::vector<Tile*> _tilesToUpdate;

	void attachTriggerCallbackToTile(Tile* tile, std::function<void(Entity*)> callback);

	void handleInput();
	void handleMovement();

	void handleLevelComplete();
	void returnToMainMenu();

	void drawHealthBar(std::shared_ptr<sf::RenderWindow> window);
	void loadControls();

	bool hasKeys();
};
