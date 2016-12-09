#pragma once

#include "GamePhase.h"
#include "Tile.h"

#include <memory>

#include <SFML/Window/Keyboard.hpp>

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

	static sf::Vector2f arenaTunnelEntrance;
	static sf::Vector2f arenaTunnelExit;

	static sf::Vector2f bronzeKeyUnlockTile;
	static sf::Vector2f bronzeKeyGateTile;

	static sf::Vector2f silverKeyUnlockTile;
	static sf::Vector2f silverKeyGateTile;

	static sf::Vector2f goldKeyUnlockTile;
	static sf::Vector2f goldKeyGateTile;

	virtual ~MainGame();

	void update() override;
	void render(std::shared_ptr<sf::RenderWindow> window) override;

	void handlePlayerDeath();
	bool loadLevel(const std::string& textureDefinitionFileName, const std::string& levelDefinitionFileName);

protected:
	MainGame();

private:
	bool _levelComplete;

	bool _escapePressed;

	std::vector<Tile*> _tilesToUpdate;
	std::vector<Tile*> _prevTiles;
	sf::Vector2f _prevPlayerPos;

	std::function<void(Entity*)> createUnlockCallback(const std::string& keyName, const sf::Vector2f& gatePosition);
	void attachTriggerCallbackToTile(Tile* tile, std::function<void(Entity*)> callback);
	void broadcastGameOverMessage();
	void handleInput();
	void handleMovement();

	void handleLevelComplete();
	void returnToMainMenu();

	void loadControls();
};
