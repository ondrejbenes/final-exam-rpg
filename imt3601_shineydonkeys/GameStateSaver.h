#pragma once
class GameStateSaver
{
public:
	GameStateSaver();
	virtual ~GameStateSaver();

	void saveGame();
};