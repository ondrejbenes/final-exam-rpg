#pragma once
class GamePhase
{

public:
	GamePhase();
	virtual ~GamePhase();

	bool initialize();
	void render();
	void update();

};