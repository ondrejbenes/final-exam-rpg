#pragma once
#include "UI.h"

class GamePhase
{

public:
	UI ui;

	GamePhase();
	virtual ~GamePhase();

	bool initialize();
	void render();
	void update();
};
