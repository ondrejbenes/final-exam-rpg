#include "EntityFactory.h"

#include <regex>

// TODO use XML instead?
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
	gc->addSprite("item", sprite);
	gc->setActiveSprite("item");
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
	gc->addSprite("item", sprite);
	gc->setActiveSprite("item");

	return item;
}

Character* EntityFactory::createDonkey() 
{
	CharacterStats stats{1,1};
	auto donkey = new Character(stats);

	// TODO ResLoader
	auto texture = new sf::Texture;
	if (!texture->loadFromFile("Resources/Images/Donkey.png"))
		LOG_E("Error: could not load donkey image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	auto gc = new GraphicsComponent(*donkey);
	gc->addSprite("donkey", sprite);
	gc->setActiveSprite("donkey");
	donkey->addComponent(gc);

	auto pc = new PhysicsComponent(*donkey);
	pc->setCollider(sprite.getGlobalBounds());
	donkey->addComponent(pc);
	
	return donkey;
}

Npc* EntityFactory::createNpcFromXml(const tinyxml2::XMLElement& element) 
{
	// create instance

	// load stats

	// create weapon

	// create inventory items

	// load texture

	// create sprite

	// create components (consider moving to constructor)

	// set position

	// set AI state

}

Player* EntityFactory::createPlayerFromXml(const tinyxml2::XMLElement& element) 
{

	// create instance

	// load stats

	// create weapon

	// create inventory items

	// load textures (move, combat)

	// create sprites (move, combat)

	// create components (consider moving to constructor)

	// set position

}