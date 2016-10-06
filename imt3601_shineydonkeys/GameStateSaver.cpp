#include "GameStateSaver.h"
#include <fstream>
#include "TimeUtilities.h"
#include <sstream>
#include "EntityManager.h"

GameStateSaver::GameStateSaver(){

}

GameStateSaver::~GameStateSaver(){

}

void GameStateSaver::saveGame()
{
	auto localPlayer = EntityManager::localPlayer;

	std::stringstream ss;
	ss <<
		TimeUtilities::getCurrentTimeAndDate() << "\n" <<
		localPlayer->getPosition().x << ";" <<
		localPlayer->getPosition().y;
	std::ofstream ofs;
	ofs.open("save1.txt", std::ofstream::trunc);
	ofs << ss.str();
	ofs.close();
}