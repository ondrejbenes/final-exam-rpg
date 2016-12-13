#include "EntityFactory.h"

#include <regex>
#include "LevelLoader.h"
#include "EntityManager.h"
#include "AiPatrol.h"
#include "AiIdle.h"

// TODO use XML instead?
Entity* EntityFactory::createFromToString(std::string str)
{
	/*std::regex entityRegEx("Type: class ([[:w:]]+), Id: .*, Pos: ([[:d:]]+\.?[[:d:]]*);([[:d:]]+\.?[[:d:]]*)");
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
	return entity;*/
	return nullptr;
}

void EntityFactory::addWeaponInventorySprite(std::shared_ptr<Item> weapon, const std::string& texturePath)
{
	auto texture = ResourceLoader::getInstance()->getTexture(texturePath);
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

	auto texture = ResourceLoader::getInstance()->getTexture(texturePath);
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

	auto texture = ResourceLoader::getInstance()->getTexture("Resources/Images/Donkey.png");
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
	// load stats
	CharacterStats stats;
	stats.max_hitpoints = std::stoi(element.FirstChildElement("HitPoints")->GetText());
	stats.current_hitpoints = std::stoi(element.FirstChildElement("HitPoints")->GetText());

	auto npc = new Npc(stats);

	auto positionX = std::stof(element.FirstChildElement("Position")->Attribute("x"));
	auto positionY = std::stof(element.FirstChildElement("Position")->Attribute("y"));
	//auto usedWeapon = element.FirstChildElement("UsedWeapon")->Attribute("name");

	// set position
	npc->setPosition(sf::Vector2f(positionX, positionY));

	auto usedWeapon = element.FirstChildElement("UsedWeapon")->Attribute("name");
	auto entityManager = EntityManager::getInstance();
	npc->setEquipedWeapon(std::dynamic_pointer_cast<Weapon>(entityManager->getItemByName(usedWeapon)));
	
	// set inventory items
	auto inventory = element.FirstChildElement("Inventory");
	if(inventory != nullptr)
	{
		for (auto itemXml = inventory->FirstChildElement("Item"); itemXml; itemXml = itemXml->NextSiblingElement("Item"))
		{
			auto itemName = itemXml->Attribute("name");
			auto& item = entityManager->getItemByName(itemName);
			npc->getInventory().push_back(item);
		}		
	}

	// create components (consider moving to constructor)
	npc->addComponent(new AnimationComponent(*npc));

	// load texture
	auto NPCspriteLoc = element.FirstChildElement("Sprite")->Attribute("link");
	auto texture = ResourceLoader::getInstance()->getTexture(NPCspriteLoc);

	// create sprite
	sf::Sprite sprite;
	sprite.setTexture(*texture);
	auto gc = new GraphicsComponent(*npc);
	gc->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, sf::Vector2u(4, 4));
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
	npc->addComponent(gc);

	if (!Network::isMultiplayer() || Network::isServer())
	{
		auto ac = new AiComponent(*npc);
		npc->addComponent(ac);
		auto aiStateXml = element.FirstChildElement("AiState");
		std::string aiState = aiStateXml->Attribute("type");
		auto radius = aiStateXml->Attribute("radius");
		if(aiState == "Patrol")
		{
			ac->ChangeState(new AiPatrol(ac, npc->getPosition(), std::stof(radius)));
		} 
		else
		{
			ac->ChangeState(new AiIdle(ac, std::stof(radius)));			
		}
	}

	npc->addComponent(new CombatComponent(*npc));

	auto pc = new PhysicsComponent(*npc);
	auto colliderRect = sf::FloatRect(sprite.getGlobalBounds());
	colliderRect.width /= 4;
	colliderRect.height /= 4;
	pc->setCollider(colliderRect);
	npc->addComponent(pc);

	return npc;
}

Player* EntityFactory::createPlayerFromXml(const tinyxml2::XMLElement& element) 
{
	// load stats
	CharacterStats stats;
	stats.max_hitpoints = std::stoi(element.FirstChildElement("HitPoints")->GetText());
	stats.current_hitpoints = std::stoi(element.FirstChildElement("HitPoints")->GetText());

	auto player = new Player(stats);

	auto positionX = std::stof(element.FirstChildElement("Position")->Attribute("x"));
	auto positionY = std::stof(element.FirstChildElement("Position")->Attribute("y"));
	//auto usedWeapon = element.FirstChildElement("UsedWeapon")->Attribute("name");

	// set position
	player->setPosition(sf::Vector2f(positionX, positionY));

	auto usedWeapon = element.FirstChildElement("UsedWeapon")->Attribute("name");
	auto entityManager = EntityManager::getInstance();
	player->setEquipedWeapon(std::dynamic_pointer_cast<Weapon>(entityManager->getItemByName(usedWeapon)));

	// set inventory items
	auto inventory = element.FirstChildElement("Inventory");
	if (inventory != nullptr)
	{
		for (auto itemXml = inventory->FirstChildElement("Item"); itemXml; itemXml = itemXml->NextSiblingElement("Item"))
		{
			auto itemName = itemXml->Attribute("name");
			auto& item = entityManager->getItemByName(itemName);
			player->getInventory().push_back(item);
		}
	}

	// create components (consider moving to constructor)
	player->addComponent(new AnimationComponent(*player));

	auto currSpriteNumber = ConfigIO::readInt(L"player", L"sprite", 1);
	std::stringstream ss;
	ss << "Resources/Images/Char_Animations/Player" << currSpriteNumber << ".png";

	auto texture = ResourceLoader::getInstance()->getTexture(ss.str());
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	ss.seekp(-4, ss.cur);
	ss << "_Battle.png";

	auto combatTexture = ResourceLoader::getInstance()->getTexture(ss.str());
	sf::Sprite combatSprite;
	combatSprite.setTexture(*combatTexture);

	auto gc = new GraphicsComponent(*player);
	gc->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, sf::Vector2u(4, 4));
	gc->addSprite(CombatComponent::COMBAT_SPRITE_NAME, combatSprite, sf::Vector2u(4, 4));
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
	player->addComponent(gc);

	player->addComponent(new CombatComponent(*player));

	auto pc = new PhysicsComponent(*player);
	auto colliderRect = sf::FloatRect(sprite.getGlobalBounds());
	colliderRect.width /= 4;
	colliderRect.height /= 4;
	pc->setCollider(colliderRect);
	player->addComponent(pc);


	return player;
}