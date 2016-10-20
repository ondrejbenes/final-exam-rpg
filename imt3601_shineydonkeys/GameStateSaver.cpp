#include <fstream>
#include <sstream>

#include "GameStateSaver.h"
#include "TimeUtilities.h"
#include "EntityManager.h"

GameStateSaver::GameStateSaver()
{

}

GameStateSaver::~GameStateSaver()
{

}

void GameStateSaver::quickSave()
{
	std::stringstream ss;
	ss << TimeUtilities::getCurrentTimeAndDate() << "\n";

	auto characters = EntityManager::getInstance()->getAllCharacters();
	for (auto it = characters.begin(); it != characters.end(); ++it)
		ss << (*it)->toString() << "\n";

	auto str = ss.str();
	str.erase(str.find_last_not_of("\n") + 1);

	std::ofstream ofs;
	ofs.open("quickSave.txt", std::ofstream::trunc);
	ofs << str;
	ofs.close();
}