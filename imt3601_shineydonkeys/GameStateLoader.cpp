#include "GameStateLoader.h"
#include <fstream>
#include "Logger.h"
#include <regex>
#include "EntityFactory.h"
#include "EntityManager.h"

void GameStateLoader::quickLoad()
{
	// TODO reimplement to use XML
	//std::ifstream ifs("quickSave.txt");

	//if (!ifs.is_open())
	//{
	//	LOG_W("QuickSave file not found");
	//	return;
	//}
	//auto entityManager = EntityManager::getInstance();
	//entityManager->clearCharacters();

	//// Regex usage - http://stackoverflow.com/questions/11627440/regex-c-extract-substring
	//// TODO extract entity regex somewhere (duplication in entityfactory)
	//std::regex entityRegEx("Type: class (.*), Id: .*, Pos: (.*);(.*)");
	//std::smatch match;

	//EntityFactory factory;
	//std::string line;
	//while (getline(ifs, line))
	//{
	//	if (regex_search(line, match, entityRegEx))
	//	{
	//		auto entity = factory.createFromToString(line);

	//		if(entity != nullptr)
	//		{
	//			if(typeid(*entity) == typeid(Player))
	//			{
	//				entityManager->setLocalPlayer(dynamic_cast<Player*>(entity));
	//				entityManager->add(entity);
	//			}

	//			if (typeid(*entity) == typeid(Npc))
	//			{
	//				entityManager->add(entity);
	//			}
	//		}
	//	}
	//}

	//ifs.close();
}