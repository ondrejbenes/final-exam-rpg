#include "CombatComponent.h"
#include "EntityManager.h"
#include "Blackboard.h"
#include "Game.h"
#include "DamageSplash.h"
#include "GamePhaseManager.h"
#include "Audio.h"
#include "ChatBoard.h"
#include "PacketFactory.h"
#include "Network.h"

#include <sstream>
#include "PhysicsComponent.h"
#include "GraphicsComponent.h"
#include "MainGame.h"

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

	auto pc = parent.getComponent<PhysicsComponent>();
	auto gc = parent.getComponent<GraphicsComponent>();

	if (pc->getVelocity() == PhysicsComponent::ZERO_VELOCITY)
		gc->setActiveSprite(COMBAT_SPRITE_NAME);
	else
		gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

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
}

void CombatComponent::endCombat()
{
	_inCombat = false;
	_other = nullptr;
	_otherCombatComp = nullptr;

	auto gc = parent.getComponent<GraphicsComponent>();
	gc->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
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
		auto player = entityManager->getLocalPlayer();

		if (parent.id != player->id)
		{
			auto parentAsCharacter = dynamic_cast<Character*>(&getParent());
			auto loot = parentAsCharacter->getInventory();
			auto& inventoryOfOther = entityManager->getLocalPlayer()->getInventory();

			auto chatBoard = dynamic_cast<ChatBoard*>(ui.getElementByName("chatBoard"));
			for (auto it = begin(loot); it != end(loot); ++it)
			{
				auto message = (*it)->getName() + " added to inventory";;
				chatBoard->addMessage("System", message);

				Blackboard::getInstance()->leaveCallback(NETWORK,
					[message](Module* target)
					{
						PacketFactory factory;
						auto packet = factory.createChatMessage(std::string("System") + ": " + message);
						auto network = dynamic_cast<Network*>(target);
						network->broadcast(packet);
					}
				);

				inventoryOfOther.push_back(*it);
			}
		} 
		else
		{
			auto phase = dynamic_cast<MainGame*>(GamePhaseManager::getInstance()->getCurrentPhase());
			phase->handlePlayerDeath();
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

		// TODO set player to nullptr in EM?
	}
}

const std::string CombatComponent::COMBAT_SPRITE_NAME = "combat";
