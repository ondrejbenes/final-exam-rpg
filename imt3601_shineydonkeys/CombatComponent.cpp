#include "CombatComponent.h"
#include "Logger.h"
#include "EntityManager.h"
#include "Blackboard.h"
#include "Game.h"
#include "DamageSplash.h"
#include "GamePhaseManager.h"
#include "Audio.h"

#include <sstream>
#include <valarray>
#include "ChatBoard.h"

CombatComponent::CombatComponent(Entity& parent) :
EntityComponent(parent),
_inCombat(false), 
_other(nullptr), 
_otherCombatComp(nullptr)
{

}

CombatComponent::~CombatComponent()
{

}

void CombatComponent::update()
{
	if (!_inCombat)
		return;

	auto weapon = dynamic_cast<Character*>(&parent)->getEquipedWeapon();
	if (attackTimer.getElapsedTime().asMilliseconds() < weapon->getAttackSpeedMs())
		return;
	attackTimer.restart();
	auto damageModifier = rand() % (weapon->getMaxDamage() - weapon->getMinDamage());
	auto damage = weapon->getMinDamage() + damageModifier;
	_otherCombatComp->takeDamage(damage);

	Blackboard::getInstance()->leaveCallback(
		AUDIO,
		[](Module* target)
		{
			dynamic_cast<Audio*>(target)->playSound(Audio::WEAPON_SLASH);
		}
	);
}

void CombatComponent::startCombat(Character* other)
{
	_inCombat = true;
	_other = other;
	_otherCombatComp = _other->getComponent<CombatComponent>();


	//TODO move to animetionComponent and fix

	//auto texture = new sf::Texture;
	//if (!texture->loadFromFile("Resources/Images/Player3.png"))
	//	LOG_E("Error: could not load player image");
	//sf::Sprite sprite;
	//sprite.setTexture(*texture);

	//auto gc = parent.getComponent<GraphicsComponent>();
	//gc->setSprite(sprite);

}

void CombatComponent::endCombat()
{
	_inCombat = false;
	_other = nullptr;
	_otherCombatComp = nullptr;
}

void CombatComponent::takeDamage(const unsigned int damage)
{
	std::stringstream ss;
	auto parent = getParent();

	auto stats = dynamic_cast<Character*>(&getParent())->getStats(); 
	stats->current_hitpoints = (damage > stats->current_hitpoints) ? 0U : (stats->current_hitpoints - damage);

	auto x = parent.getPosition().x;
	auto y = parent.getPosition().y;

	if (_other->getPosition().x > parent.getPosition().x)
		x -= 25;
	else
		x += 25;

	// TODO memory leak
	auto& ui = GamePhaseManager::getInstance()->getCurrentPhase()->getUi();
	ui.addElement(new DamageSplash(damage, x, y));

	if (stats->current_hitpoints == 0)
	{
		auto entityManager = EntityManager::getInstance();
		if(parent.id == entityManager->getLocalPlayer()->id)
		{
			// handlePlayerDeath();
		}
		else
		{
			auto parentAsCharacter = dynamic_cast<Character*>(&getParent());
			auto loot = parentAsCharacter->getInventory();
			auto& inventoryOfOther = entityManager->getLocalPlayer()->getInventory();

			auto chatBoard = dynamic_cast<ChatBoard*>(ui.getElementByName("chatBoard"));
			for (auto it = begin(loot); it != end(loot); ++it)
			{
				chatBoard->addMessage("System", (*it)->getName() + " added to inventory");
				inventoryOfOther.push_back(*it);
			}			

			Blackboard::getInstance()->leaveCallback(
				AUDIO,
				[](Module* target)
				{
					dynamic_cast<Audio*>(target)->playSound(Audio::HUMAN_NPC_DYING);
				}
			);

			if (_otherCombatComp != nullptr && _otherCombatComp->getOther()->id == parent.id)
				_otherCombatComp->endCombat();
			endCombat();
			entityManager->remove(&parent);
		}
	}
}
