#include "GameStateLoader.h"
#include <fstream>
#include "Logger.h"
#include <regex>
#include "EntityFactory.h"
#include "EntityManager.h"

GameStateLoader::GameStateLoader()
{

}

GameStateLoader::~GameStateLoader()
{

}

void GameStateLoader::quickLoad()
{
	std::ifstream ifs("quickSave.txt");

	if (!ifs.is_open())
	{
		LOG_W("QuickSave file not found");
		return;
	}

	EntityFactory factory;
	EntityManager::clearCharacters();

	// Regex usage - http://stackoverflow.com/questions/11627440/regex-c-extract-substring
	// TODO extract entity regex somewhere (duplication in entityfactory)
	std::regex entityRegEx("Type: class (.*), Id: .*, Pos: (.*);(.*)");
	std::smatch match;

	std::string line;
	while (getline(ifs, line))
	{
		if (regex_search(line, match, entityRegEx))
		{
			auto entity = factory.createFromToString(line);

			if(entity != nullptr)
			{
				if(typeid(*entity) == typeid(Player))
				{
					// TODO if we have multiplayer, we may have problems here (modifying save file structure may be easiest fix):
					EntityManager::localPlayer = dynamic_cast<Player*>(entity);
					EntityManager::gameEntities.push_back(entity);
				}

				if (typeid(*entity) == typeid(Npc))
				{
					EntityManager::gameEntities.push_back(entity);
				}
			}
		}
	}

	ifs.close();
}