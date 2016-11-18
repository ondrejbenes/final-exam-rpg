#include "EntityFactory.h"
#include <regex>

EntityFactory::EntityFactory(){

}

EntityFactory::~EntityFactory(){

}

Entity* EntityFactory::createFromToString(std::string str)
{
	std::regex entityRegEx("Type: class ([[:w:]]+), Id: .*, Pos: ([[:d:]]+\.?[[:d:]]*);([[:d:]]+\.?[[:d:]]*)");
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

void EntityFactory::initWeapon(std::shared_ptr<Item> weapon, const std::string& texturePath)
{
	auto texture = new sf::Texture();
	if (!texture->loadFromFile(texturePath))
		LOG_E("Error loading item icon texture");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*weapon);
	weapon->addComponent(gc);
	gc->setSprite(sprite);
}

std::shared_ptr<Item> EntityFactory::createInventoryItem(const std::string& texturePath)
{
	auto item = std::make_shared<Item>();

	auto texture = new sf::Texture();
	if (!texture->loadFromFile(texturePath))
		LOG_E("Error loading item icon texture");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*item);
	item->addComponent(gc);
	gc->setSprite(sprite);

	return item;
}
