#include "EntityFactory.h"
#include <regex>

EntityFactory::EntityFactory(){

}

EntityFactory::~EntityFactory(){

}

Entity* EntityFactory::createFromToString(std::string str)
{
	std::regex entityRegEx("Type: class ([[:w:]]+), Id: .*, Pos: ([[:d:]]+);([[:d:]]+)");
	std::smatch match;

	Entity* entity = nullptr;

	if(std::regex_search(str, match, entityRegEx))
	{
		auto type = match[1].str();

		if (type == "Player")
			entity = create<Player>();
		else if (type == "Npc")
			entity = create<Npc>();

		if(entity != nullptr)
			entity->setPosition(sf::Vector2f(stof(match[2]), stof(match[3])));
		else
			LOG_D("Unknown entity type");
	}
	return entity;
}