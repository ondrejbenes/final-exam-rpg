#pragma once

#include "Npc.h"
#include "Player.h"
#include "UnsopportedEntityException.h"
#include "GraphicsComponent.h"
#include "PhysicsComponent.h"
#include "Logger.h"
#include "Tile.h"
#include "AiComponent.h"
#include "AnimationComponent.h"
#include "CombatComponent.h"
#include "Weapon.h"
#include "Network.h"
#include "ConfigIO.h"

#include <sstream>
#include "tinyxml2.h"
#include "ResourceLoader.h"

class EntityFactory
{
public:	
	Entity* createFromToString(std::string str);
	Npc* createNpcFromXml(const tinyxml2::XMLElement& element);
	Player* createPlayerFromXml(const tinyxml2::XMLElement& element);
	Character* createDonkey();
	//Npc* createNpcFromXml(const tinyxml2::XMLElement& element, const tinyxml2::XMLAttribute& weaponName);
	void addWeaponInventorySprite(std::shared_ptr<Item> weapon, const std::string& texturePath);
	std::shared_ptr<Item> createInventoryItem(const std::string& texturePath);
};