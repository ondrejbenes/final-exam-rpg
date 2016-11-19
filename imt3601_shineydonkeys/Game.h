#pragma once

#include "Module.h"
#include "GameState.h"

class Game : public Module
{
public:
	GameState gameState;

	Game();
	virtual ~Game();
	bool initialize() override;
	void update() override;

	void quickSave();
	void quickLoad();

	void stop();
	bool isRunning() const { return _running; }
private:
	bool _running;
};
