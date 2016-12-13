#include "MainGame.h"
#include "Npc.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "Tilemap.h"
#include "Game.h"
#include "Blackboard.h"
#include "GamePhaseManager.h"
#include "ConfigIO.h"
#include "Console.h"
#include "AiPatrol.h"
#include "windows.h"
#include "Network.h"
#include "PacketFactory.h"
#include "Audio.h"
#include "ChatBoard.h"
#include "Renderer.h"
#include "VectorUtilities.h"
#include "LevelLoader.h"
#include "Scheduler.h"

#include <memory>
#include "AiIdle.h"

using namespace std::chrono_literals;

MainGame::MainGame() :
	_levelComplete(false),
	_escapePressed(false),
	_prevPlayerPos(sf::Vector2f())
{
	auto cursor = GetCursor();
	SetCursor(LoadCursor(nullptr, IDC_WAIT));


	// TODO load from XML
	Tilemap::MAP_WIDTH = 8000;
	Tilemap::MAP_HEIGHT = 8000;

	Entity::nextId = 0;

	EntityFactory factory;
	auto player = factory.create<Player>();
	auto entityManager = EntityManager::getInstance();
	entityManager->clearCharacters();
	entityManager->add(player);
	player->setPosition(sf::Vector2f(562, 1807));
	entityManager->setLocalPlayer(player);


	//Normal Npc
	
	auto npc1 = factory.create<Npc>();
	auto weapon = std::make_shared<Weapon>(7, 14, 1250);
	npc1->setEquipedWeapon(weapon);
	auto sword = std::make_shared<Weapon>(15, 25, 750);
	sword->setName("Scimitar");
	factory.initWeapon(sword, "Resources/Images/Weapons/Scimitar.png");
	npc1->getInventory().push_back(sword);

	auto texture = ResourceLoader::getInstance()->getTexture("Resources/Images/Npc1.png");
	sf::Sprite sprite;
	sprite.setTexture(*texture);

	npc1->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite, sf::Vector2u(4, 4));
	npc1->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	auto pos = sf::Vector2f(633, 491);
	npc1->setPosition(pos);
	auto ac = npc1->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiIdle(ac, 500));
	entityManager->add(npc1);

	npc1->getStats().max_hitpoints = 100;
	npc1->getStats().current_hitpoints = 100;


	//First mini boss
	auto npc_firstMiniBoss = factory.create<Npc>();

	auto weapon2 = std::make_shared<Weapon>(10, 20, 1100);
	npc_firstMiniBoss->setEquipedWeapon(weapon2);
	auto key = factory.createInventoryItem("Resources/Images/Keys/BronzeKey.png");
	key->setName("Bronze Key");
	npc_firstMiniBoss->getInventory().push_back(key);
	
	ac = npc_firstMiniBoss->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));
	entityManager->add(npc_firstMiniBoss);

	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/Npc2.png");
	sf::Sprite sprite2;
	sprite2.setTexture(*texture);

	npc_firstMiniBoss->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite2, sf::Vector2u(4, 4));
	npc_firstMiniBoss->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);
	pos = sf::Vector2f(1388, 3400);
	npc_firstMiniBoss->setPosition(pos);
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));
	entityManager->add(npc_firstMiniBoss);

	npc_firstMiniBoss->getStats().max_hitpoints = 150;
	npc_firstMiniBoss->getStats().current_hitpoints = 150;



	//Second mini boss
	auto npc_secondMiniBoss = factory.create<Npc>();
	auto weapon3 = std::make_shared<Weapon>(10, 20, 800);
	npc_secondMiniBoss->setEquipedWeapon(weapon3);
	auto skey = factory.createInventoryItem("Resources/Images/Keys/SilverKey.png");
	skey->setName("Silver Key");
	npc_secondMiniBoss->getInventory().push_back(skey);

	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/Npc3.png");
	sf::Sprite sprite3;
	sprite3.setTexture(*texture);

	npc_secondMiniBoss->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite3, sf::Vector2u(4, 4));
	npc_secondMiniBoss->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	pos = sf::Vector2f(3618, 1282);
	npc_secondMiniBoss->setPosition(pos);
	ac = npc_secondMiniBoss->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));
	entityManager->add(npc_secondMiniBoss);

	npc_secondMiniBoss->getStats().max_hitpoints = 200;
	npc_secondMiniBoss->getStats().current_hitpoints = 200;



	//Third mini boss
	auto npc_thirdMiniBoss = factory.create<Npc>();

	auto weapon4 = std::make_shared<Weapon>(5, 50, 750);
	npc_thirdMiniBoss->setEquipedWeapon(weapon4);
	auto gkey = factory.createInventoryItem("Resources/Images/Keys/GoldKey.png");
	gkey->setName("Gold Key");
	npc_thirdMiniBoss->getInventory().push_back(gkey);
	auto axe = std::make_shared<Weapon>(50, 100, 500);
	factory.initWeapon(axe, "Resources/Images/Weapons/Axe.png");
	axe->setName("Axe");
	npc_thirdMiniBoss->getInventory().push_back(axe);

	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/Npc4.png");
	sf::Sprite sprite4;
	sprite4.setTexture(*texture);

	npc_thirdMiniBoss->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite4, sf::Vector2u(4, 4));
	npc_thirdMiniBoss->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	pos = sf::Vector2f(4304, 3359);
	npc_thirdMiniBoss->setPosition(pos);
	ac = npc_thirdMiniBoss->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));
	entityManager->add(npc_thirdMiniBoss);

	npc_thirdMiniBoss->getStats().max_hitpoints = 300;
	npc_thirdMiniBoss->getStats().current_hitpoints = 300;




	//DeamonDog1
	auto npc_deamonDog = factory.create<Npc>();

	auto weapon_DD = std::make_shared<Weapon>(3, 5, 350);
	npc_deamonDog->setEquipedWeapon(weapon4);

	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/SmallDeamon.png");
	sf::Sprite sprite_DD;
	sprite_DD.setTexture(*texture);

	npc_deamonDog->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite_DD, sf::Vector2u(4, 4));
	npc_deamonDog->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	pos = sf::Vector2f(7168, 4096);
	npc_deamonDog->setPosition(pos);
	ac = npc_deamonDog->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 100));
	entityManager->add(npc_deamonDog);

	npc_deamonDog->getStats().max_hitpoints = 700;
	npc_deamonDog->getStats().current_hitpoints = 700;



//DeamonDog2
	auto npc_deamonDog_2 = factory.create<Npc>();

	auto weapon_DD2 = std::make_shared<Weapon>(3, 5, 350);
	npc_deamonDog_2->setEquipedWeapon(weapon_DD2);
	
	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/SmallDeamon.png");
	sf::Sprite sprite_DD2;
	sprite_DD2.setTexture(*texture);

	npc_deamonDog_2->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite_DD2, sf::Vector2u(4, 4));
	npc_deamonDog_2->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	pos = sf::Vector2f(7392, 4096);
	npc_deamonDog_2->setPosition(pos);
	ac = npc_deamonDog_2->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 100));
	entityManager->add(npc_deamonDog_2);

	npc_deamonDog_2->getStats().max_hitpoints = 700;
	npc_deamonDog_2->getStats().current_hitpoints = 700;

///Boss
	auto npc_Boss_2 = factory.create<Npc>();

	auto weapon_Boss = std::make_shared<Weapon>(30, 80, 2500);
	npc_Boss_2->setEquipedWeapon(weapon_Boss);

	texture = ResourceLoader::getInstance()->getTexture("Resources/Images/BossDeamon.png");
	sf::Sprite sprite_Boss;
	sprite_Boss.setTexture(*texture);

	npc_Boss_2->getComponent<GraphicsComponent>()->addSprite(PhysicsComponent::MOVE_SPRITE_NAME, sprite_Boss, sf::Vector2u(4, 4));
	npc_Boss_2->getComponent<GraphicsComponent>()->setActiveSprite(PhysicsComponent::MOVE_SPRITE_NAME);

	pos = sf::Vector2f(7264, 4416);
	npc_Boss_2->setPosition(pos);
	ac = npc_Boss_2->getComponent<AiComponent>();
	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 250));
	entityManager->add(npc_Boss_2);

	npc_Boss_2->getStats().max_hitpoints = 1500;
	npc_Boss_2->getStats().current_hitpoints = 1500;







	auto donkey = factory.createDonkey();
	donkey->setPosition(sf::Vector2f(724, 1555));
	donkey->setName("donkey");

	auto boundaryPos = donkey->getPosition() - sf::Vector2f(100, 100);
	auto donkeySpriteBounds = donkey->getComponent<GraphicsComponent>()->getActiveSprite().getGlobalBounds();
	auto boundarySize = sf::Vector2f(donkeySpriteBounds.width + 200, donkeySpriteBounds.height + 200);
	auto donkeyTextTrigger = std::make_shared<Trigger>(sf::FloatRect(boundaryPos, boundarySize));
	auto callback = std::make_shared<std::function<void(Entity*)>>(
		[](Entity* enteringEntity)
	{
		if (typeid(*enteringEntity) != typeid(Player))
			return;

		if (MainGame::donkeyTextShown)
			return;

		MainGame::donkeyTextShown = true;

		auto mainGame = GamePhaseManager::getInstance()->getCurrentPhase();

		auto chatBoard = mainGame->getUi().getElementByName<ChatBoard>("chatBoard");

		chatBoard->addMessage("Shiny Donkey", "Hi. I'm the Shiney Donkey");

		Blackboard::getInstance()->leaveCallback(
			SCHEDULER,
			[chatBoard](Module* target)
		{
			auto scheduler = dynamic_cast<Scheduler*>(target);
			auto msg1 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You won't believe it, but I have a quest for you."); };
			scheduler->schedule(msg1, NOW + 2s);
			auto msg2 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You need to explore the island and look for keys."); };
			scheduler->schedule(msg2, NOW + 4s);
			auto msg3 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "The keys will unlock the gate to the east of here."); };
			scheduler->schedule(msg3, NOW + 6s);
			auto msg4 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "Then I need you to enter the tunnel."); };
			scheduler->schedule(msg4, NOW + 8s);
			auto msg5 = [chatBoard]() {chatBoard->addMessage("Shiny Donkey", "You will know what to do when you exit on the other side."); };
			scheduler->schedule(msg5, NOW + 10s);
		}
		);
	});
	donkeyTextTrigger->setOnTriggerEnter(callback);
	donkey->getComponent<PhysicsComponent>()->getTriggers().push_back(donkeyTextTrigger);
	entityManager->add(donkey);

#ifdef  _DEBUG
	loadLevel("Resources/Images/tilesTESTING.png", "Resources/Levels/FinalExamTileMapTESTING.csv");
#else
	loadLevel("Resources/Images/tiles.png", "Resources/Levels/THISFinalExamMap.csv");
#endif

	loadControls();
	SetCursor(cursor);

#ifndef _DEBUG
	auto arenaTeleportTile = entityManager->getTileAtPos(arenaTunnelEntrance);
	attachTriggerCallbackToTile(arenaTeleportTile,
		[](Entity* enteringEntity)
	{
		auto player = EntityManager::getInstance()->getLocalPlayer();
		if (player->id != enteringEntity->id)
			return;

		Blackboard::getInstance()->leaveCallback(
			RENDERER,
			[](Module* target)
		{
			dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You found a tunnel and gathered the courage to go forward...");
		}
		);

		Blackboard::getInstance()->leaveCallback(
			SCHEDULER,
			[](Module* target)
		{
			auto lambda = []()
			{
				Blackboard::getInstance()->leaveCallback(
					RENDERER,
					[](Module* target)
				{
					dynamic_cast<Renderer*>(target)->fadeIn(sf::seconds(3), "The tunnel lead you to an arena filled with deamons!");
					auto entityManager = EntityManager::getInstance();
					auto player = entityManager->getLocalPlayer();
					entityManager->move(player, arenaTunnelExit);
					player->setPosition(arenaTunnelExit);
				});
			};
			dynamic_cast<Scheduler*>(target)->schedule(lambda, NOW + 2900ms);
		}
		);
	});

	auto bronzeKeyGateTile = entityManager->getTileAtPos(bronzeKeyGateTilePos);
	attachTriggerCallbackToTile(bronzeKeyGateTile, createUnlockCallback("Bronze Key", bronzeKeyGateTilePos, 168)); // TODO new tile type in XML

	auto silverKeyGateTile = entityManager->getTileAtPos(silverKeyGateTilePos);
	attachTriggerCallbackToTile(silverKeyGateTile, createUnlockCallback("Silver Key", silverKeyGateTilePos, 59)); // TODO new tile type in XML

	auto goldKeyGateTile = entityManager->getTileAtPos(goldKeyGateTilePos);
	attachTriggerCallbackToTile(goldKeyGateTile, createUnlockCallback("Gold Key", goldKeyGateTilePos, 59)); // TODO new tile type in XML

	Blackboard::getInstance()->leaveCallback(
		AUDIO,
		[](Module* target)
	{
		dynamic_cast<Audio*>(target)->playMusic(Audio::THEME_SONG);
	}
	);
#endif // notdef _DEBUG

	Blackboard::getInstance()->leaveCallback(
		RENDERER,
		[](Module* target)
	{
		dynamic_cast<Renderer*>(target)->fadeIn(sf::seconds(4), "You wake up on a beach, not remembering how you got there.");
	}
	);
}

MainGame::~MainGame()
{
	EntityManager::getInstance()->clearCharacters();
}

void MainGame::update()
{
	if (_levelComplete)
		return;

	auto entityManager = EntityManager::getInstance();

	auto characters = entityManager->getAllCharacters();

	handleInput();

	for (auto it = characters.begin(); it != characters.end(); ++it)
	{
		if ((*it)->getStats().current_hitpoints != 0)
			(*it)->update();
	}

	for (auto it = _tilesToUpdate.begin(); it != _tilesToUpdate.end(); ++it)
		(*it)->update();

	// TODO find out why first mini boss does not get removed
	if (characters.size() == 3 && !_levelComplete) // 2 - player, donkey
		handleLevelComplete();

	if (_escapePressed)
		returnToMainMenu();
}

void MainGame::render(std::shared_ptr<sf::RenderWindow> window)
{
	auto entityManager = EntityManager::getInstance();
	auto player = entityManager->getLocalPlayer();
	if (player == nullptr)
		return;
	auto playerPos = player->getPosition();

	auto width = window->getSize().x / 1.9;
	auto height = window->getSize().y / 1.8;
	auto boundary = QuadTreeBoundary(playerPos.x - width, playerPos.x + width, playerPos.y - height, playerPos.y + height);
	std::vector<Tile*> tiles;

	if(playerPos == _prevPlayerPos)
		tiles = _prevTiles;
	else
		tiles = entityManager->getTilesInInterval(boundary);
	for (auto it = tiles.begin(); it != tiles.end(); ++it)
	{
		auto graphicsComponent = dynamic_cast<Entity*>(*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(window);
	}
	_prevTiles = tiles;
	_prevPlayerPos = playerPos;

	auto characters = entityManager->getCharactersInInterval(boundary);
	for (auto it = characters.begin(); it != characters.end(); ++it)
	{
		auto graphicsComponent = (*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
		{
			graphicsComponent->draw(window);
			
			if (typeid(**it) == typeid(Npc))
			{
				if ((*it)->getComponent<CombatComponent>()->isInCombat())
				{
					auto& sprite = graphicsComponent->getActiveSprite();
					auto x = sprite.getPosition().x;
					auto y = sprite.getPosition().y - 10;
					auto fullHpRect = sf::RectangleShape(sf::Vector2f(sprite.getGlobalBounds().width, 4));
					fullHpRect.setFillColor(sf::Color::Red);
					fullHpRect.setPosition(x, y);

					auto parentAsChar = dynamic_cast<Character*>(*it);
					auto stats = parentAsChar->getStats();
					auto percentage = stats.current_hitpoints / float(stats.max_hitpoints);
					auto currentHpRect = sf::RectangleShape(sf::Vector2f(sprite.getGlobalBounds().width * percentage, 4));
					currentHpRect.setFillColor(sf::Color::Green);
					currentHpRect.setPosition(x, y);

					window->draw(fullHpRect);
					window->draw(currentHpRect);
				}
			}
		}
	}
	
	GamePhase::render(window);
}

std::function<void(Entity*)> MainGame::createUnlockCallback(const std::string& keyName, const sf::Vector2f& gatePosition, unsigned newTileType) 
{
	return [keyName, gatePosition, newTileType](Entity* enteringEntity)
	{
		auto em = EntityManager::getInstance();
		auto player = em->getLocalPlayer();
		if (player->id != enteringEntity->id)
			return;

		auto hasKey = false;
		for (auto item : player->getInventory())
			if (item->getName() == keyName)
				hasKey = true;

		if (hasKey)
		{
			auto gateTile = dynamic_cast<Tile*>(em->getTileAtPos(gatePosition));
			gateTile->changeType(newTileType, false);
		}
	};
}

void MainGame::attachTriggerCallbackToTile(Tile* tile, std::function<void(Entity*)> callback)
{
	auto tilePc = tile->getComponent<PhysicsComponent>();
	auto offset = Tilemap::TILE_HEIGHT / 2;
	auto& col = tilePc->getCollider();
	auto boundary = sf::FloatRect(col.left - offset, col.top - offset, col.width * 2, col.height * 2);
	auto trigger = std::make_shared<Trigger>(boundary);
	trigger->setOnTriggerEnter(std::make_shared<std::function<void(Entity*)>>(callback));
	tilePc->getTriggers().push_back(trigger);
	_tilesToUpdate.push_back(tile);
}

void MainGame::handlePlayerDeath()
{
	broadcastGameOverMessage();

	Blackboard::getInstance()->leaveCallback(
		RENDERER,
		[](Module* target)
	{
		dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You died! Game over!");
	}
	);

	Blackboard::getInstance()->leaveCallback(
		SCHEDULER,
		[](Module* target)
	{
		auto lambda = []() {GamePhaseManager::getInstance()->popPhase(); };
		dynamic_cast<Scheduler*>(target)->schedule(lambda, NOW + 2900ms);
	}
	);
}

void MainGame::handleLevelComplete()
{
	broadcastGameOverMessage();

	Blackboard::getInstance()->leaveCallback(
		RENDERER,
		[](Module* target)
	{
		dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You won!");
	}
	);

	Blackboard::getInstance()->leaveCallback(
		SCHEDULER,
		[](Module* target)
	{
		auto lambda = []() {GamePhaseManager::getInstance()->popPhase(); };
		dynamic_cast<Scheduler*>(target)->schedule(lambda, NOW + 2900ms);
	});
	_levelComplete = true;
}

void MainGame::broadcastGameOverMessage()
{
	if (Network::isServer())
	{
		Blackboard::getInstance()->leaveCallback(
			NETWORK,
			[](Module* target)
		{
			PacketFactory factory;
			auto packet = factory.createGameOver();
			dynamic_cast<Network*>(target)->broadcast(packet);
		}
		);
	}
}

void MainGame::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	auto console = Console::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				_escapePressed = true;
				return;
			}

			if (Network::isMultiplayer())
				break;

			if (event.key.code == sf::Keyboard::F5)
			{
				LOG_I("Saving game");
				blackboard->leaveCallback(GAME, [](Module* target)
				{
					dynamic_cast<Game*>(target)->quickSave();
				});
			}

			if (event.key.code == sf::Keyboard::F6)
			{
				LOG_I("Loading game");
				blackboard->leaveCallback(GAME, [](Module* target)
				{
					dynamic_cast<Game*>(target)->quickLoad();
				});
			}

			if (event.key.code == sf::Keyboard::Tilde)
				console->setVisible(!console->isVisible());

			if (console->isVisible())
				console->handleEvent(event);
			break;
		case sf::Event::TextEntered:
			if (console->isVisible())
				console->handleEvent(event);
		default:
			handleGuiEvent(event);
			break;
		}
	}

	if (!console->isVisible() && (!Network::isMultiplayer() || Network::isServer()))
		handleMovement();
}

void MainGame::handleMovement()
{
	auto localPlayer = EntityManager::getInstance()->getLocalPlayer();
	auto physicsComponent = localPlayer->getComponent<PhysicsComponent>();
	auto defaultVelocity = PhysicsComponent::DEFAULT_PLAYER_VELOCITY;
	sf::Vector2f newVelocity;

	if (sf::Keyboard::isKeyPressed(CONTROLS.up))
	{
		newVelocity = sf::Vector2f(0, -1 * defaultVelocity.y);
	}
	else if (sf::Keyboard::isKeyPressed(CONTROLS.down))
	{
		newVelocity = sf::Vector2f(0, defaultVelocity.y);
	}
	else if (sf::Keyboard::isKeyPressed(CONTROLS.right))
	{
		newVelocity = sf::Vector2f(defaultVelocity.x, 0);
	}
	else if (sf::Keyboard::isKeyPressed(CONTROLS.left))
	{
		newVelocity = sf::Vector2f(-1 * defaultVelocity.x, 0);
	}
	else
	{
		newVelocity = PhysicsComponent::ZERO_VELOCITY;
	}

	if(newVelocity != physicsComponent->getVelocity())
		physicsComponent->setVelocity(newVelocity);
}

bool MainGame::loadLevel(const std::string& textureFileName, const std::string& levelDefinitionFileName)
{
	auto tilemap = new Tilemap();
	//LevelLoader();
	if (!tilemap->loadFromFile(textureFileName, levelDefinitionFileName))
	{
		LOG_E("Failed to load level");
		return  false;
	}

	return true;;
}

void MainGame::loadControls()
{
	CONTROLS.up = sf::Keyboard::Key(ConfigIO::readInt(L"controls", L"up"));
	CONTROLS.down = sf::Keyboard::Key(ConfigIO::readInt(L"controls", L"down"));
	CONTROLS.left = sf::Keyboard::Key(ConfigIO::readInt(L"controls", L"left"));
	CONTROLS.right = sf::Keyboard::Key(ConfigIO::readInt(L"controls", L"right"));
}

void MainGame::returnToMainMenu() {
	if (EntityManager::getInstance()->getLocalPlayer()->getStats().current_hitpoints == 0 || _levelComplete)
		return;

	GamePhaseManager::getInstance()->popPhase();

	if (Network::isServer())
	{
		Blackboard::getInstance()->leaveCallback(
			NETWORK,
			[](Module* target)
		{
			PacketFactory factory;
			auto packet = factory.createGameOver();
			dynamic_cast<Network*>(target)->broadcast(packet);
		}
		);
	}

	EntityManager::getInstance()->setLocalPlayer(nullptr);
}

Controls MainGame::CONTROLS = Controls();

sf::Vector2f MainGame::arenaTunnelEntrance = sf::Vector2f(3200, 3328);
sf::Vector2f MainGame::arenaTunnelExit = sf::Vector2f(7232, 3968);

sf::Vector2f MainGame::bronzeKeyGateTilePos = sf::Vector2f(3200, 2784);
sf::Vector2f MainGame::silverKeyGateTilePos = sf::Vector2f(3200, 3040);
sf::Vector2f MainGame::goldKeyGateTilePos = sf::Vector2f(3200, 3200);

bool MainGame::donkeyTextShown = false;