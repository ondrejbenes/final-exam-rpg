#include "EntityFactory.h"

#include <regex>
#include "LevelLoader.h"

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

Npc* EntityFactory::createNpcFromXml(const tinyxml2::XMLElement& element, const std::shared_ptr<Weapon>& weaponName)
{
	CharacterStats stats;
	
	
	auto positionX = std::stof(element.FirstChildElement("Position")->Attribute("x"));
	auto positionY = std::stof(element.FirstChildElement("Position")->Attribute("y"));
	//auto usedWeapon = element.FirstChildElement("UsedWeapon")->Attribute("name");

	// load stats
	stats.max_hitpoints = std::stoi(element.FirstChildElement("Hitpoints")->GetText());
	stats.current_hitpoints = stats.max_hitpoints;

	auto NPCspriteLoc = element.FirstChildElement("Sprite")->Attribute("link");

	auto npc = new Npc(stats);

	// set position
	npc->setPosition(sf::Vector2f(positionX, positionY));

	// load texture
	auto texture = new sf::Texture;
	if (!texture->loadFromFile(NPCspriteLoc))
		LOG_E("Error: could not load player image");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	npc->setEquipedWeapon(weaponName);

	//TODO: put AI state in XML 

	auto aiState = element.FirstChildElement("StartingAiState")->Attribute("Type");

	if (aiState == "1") {}
		//Change state to whatever

	

	



	

	

	



	// set inventory items


	// create sprite
	auto gc = new GraphicsComponent(*npc);
	gc->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, 4);
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
	npc->addComponent(gc);

	// create components (consider moving to constructor)
	if (!Network::isMultiplayer() || Network::isServer())
		npc->addComponent(new AiComponent(*npc));

	npc->addComponent(new CombatComponent(*npc));

	auto pc = new PhysicsComponent(*npc);
	auto colliderRect = sf::FloatRect(sprite.getGlobalBounds());
	colliderRect.width /= 4;
	colliderRect.height /= 4;
	pc->setCollider(colliderRect);
	npc->addComponent(pc);

	npc->addComponent(new AnimationComponent(*npc));

	return npc;
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

	return nullptr;
}