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
	player->setPosition(sf::Vector2f(600, 600));
	entityManager->setLocalPlayer(player);

	auto npc1 = factory.create<Npc>();
	auto pos = sf::Vector2f(250, 230);
	npc1->setPosition(pos);
	auto ac = npc1->getComponent<AiComponent>();

	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiIdle(ac, 200));

	entityManager->add(npc1);

	auto npc2 = factory.create<Npc>();
	pos = sf::Vector2f(450, 250);
	npc2->setPosition(pos);
	ac = npc2->getComponent<AiComponent>();

	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));

	entityManager->add(npc2);
	/*
	auto npc3 = factory.create<Npc>();
	pos = sf::Vector2f(750, 230);
	npc3->setPosition(pos);
	ac = npc3->getComponent<AiComponent>();

	if (!Network::isMultiplayer() || Network::isServer())
		ac->ChangeState(new AiPatrol(ac, pos, 500));

	entityManager->add(npc3);*/

	auto donkey = factory.createDonkey();
	donkey->setPosition(sf::Vector2f(900, 600));
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

		auto ui = mainGame->getUi();
		auto chatBoard = dynamic_cast<ChatBoard*>(ui.getElementByName("chatBoard"));

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
	loadLevel("Resources/Images/tiles.png", "Resources/Levels/FinalExamMap_v2.csv");
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

	// TODO remove duplicity
	auto bronzeKeyGateUnlockTile = entityManager->getTileAtPos(bronzeKeyUnlockTile);
	attachTriggerCallbackToTile(bronzeKeyGateUnlockTile,
		[](Entity* enteringEntity)
	{
		auto em = EntityManager::getInstance();
		auto player = em->getLocalPlayer();
		if (player->id != enteringEntity->id)
			return;

		auto hasKey = false;
		for (auto item : player->getInventory())
			if (item->getName() == "Bronze Key")
				hasKey = true;

		if(hasKey)
		{
			auto gateTile = dynamic_cast<Tile*>(em->getTileAtPos(bronzeKeyGateTile));
			gateTile->changeType(168, false);
		}
	});

	auto silverKeyGateUnlockTile = entityManager->getTileAtPos(silverKeyUnlockTile);
	attachTriggerCallbackToTile(silverKeyGateUnlockTile,
		[](Entity* enteringEntity)
	{
		auto em = EntityManager::getInstance();
		auto player = em->getLocalPlayer();
		if (player->id != enteringEntity->id)
			return;

		auto hasKey = false;
		for (auto item : player->getInventory())
			if (item->getName() == "Silver Key")
				hasKey = true;

		if (hasKey)
		{
			auto gateTile = dynamic_cast<Tile*>(em->getTileAtPos(silverKeyGateTile));
			gateTile->changeType(168, false);
		}
	});

	auto goldKeyGateUnlockTile = entityManager->getTileAtPos(goldKeyUnlockTile);
	attachTriggerCallbackToTile(goldKeyGateUnlockTile,
		[](Entity* enteringEntity)
	{
		auto em = EntityManager::getInstance();
		auto player = em->getLocalPlayer();
		if (player->id != enteringEntity->id)
			return;

		auto hasKey = false;
		for (auto item : player->getInventory())
			if (item->getName() == "Gold Key")
				hasKey = true;

		if (hasKey)
		{
			auto gateTile = dynamic_cast<Tile*>(em->getTileAtPos(goldKeyGateTile));
			gateTile->changeType(168, false);
		}
	});

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
		if ((*it)->getStats()->current_hitpoints != 0)
			(*it)->update();
	}

	for (auto it = _tilesToUpdate.begin(); it != _tilesToUpdate.end(); ++it)
		(*it)->update();

	//if (characters.size() == 2 && !_levelComplete) // 2 - player, donkey
	//	handleLevelComplete();

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
			graphicsComponent->draw(window);
	}

	drawHealthBar(window);

	GamePhase::render(window);
}

void MainGame::attachTriggerCallbackToTile(Tile* tile, std::function<void(Entity*)> callback)
{
	auto tilePc = tile->getComponent<PhysicsComponent>();
	auto boundary = sf::FloatRect(tilePc->getCollider());
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

// TODO create a new subclass of UiElement instead
void MainGame::drawHealthBar(std::shared_ptr<sf::RenderWindow> window)
{
	auto width = window->getSize().x / 4;
	auto height = 48;
	auto view = window->getView();
	auto x = view.getCenter().x - view.getSize().x / 2;
	auto y = view.getCenter().y + view.getSize().y / 2 - height;

	auto fullHpRect = sf::RectangleShape(sf::Vector2f(width, height));
	fullHpRect.setFillColor(sf::Color::Red);
	fullHpRect.setPosition(x, y);

	auto player = EntityManager::getInstance()->getLocalPlayer();
	auto stats = player->getStats();
	auto percentage = stats->current_hitpoints / float(stats->max_hitpoints);
	auto currentHpRect = sf::RectangleShape(sf::Vector2f(width * percentage, height));
	currentHpRect.setFillColor(sf::Color::Green);
	currentHpRect.setPosition(x, y);

	window->draw(fullHpRect);
	window->draw(currentHpRect);
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
			handleTextEntered(event);
			break;
		case sf::Event::MouseButtonReleased:
			handleMouseReleased(event);
			break;
		default:
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
}

Controls MainGame::CONTROLS = Controls();

// TODO remove unlock tiles, change trigger areas
sf::Vector2f MainGame::arenaTunnelEntrance = sf::Vector2f(3200, 3296);
sf::Vector2f MainGame::arenaTunnelExit = sf::Vector2f(6016, 4288);

sf::Vector2f MainGame::bronzeKeyUnlockTile = sf::Vector2f(3200, 2752);
sf::Vector2f MainGame::bronzeKeyGateTile = sf::Vector2f(3200, 2784);

sf::Vector2f MainGame::silverKeyUnlockTile = sf::Vector2f(3200, 2912);
sf::Vector2f MainGame::silverKeyGateTile = sf::Vector2f(3200, 2944);

sf::Vector2f MainGame::goldKeyUnlockTile = sf::Vector2f(3200, 3168);
sf::Vector2f MainGame::goldKeyGateTile = sf::Vector2f(3200, 3200);

bool MainGame::donkeyTextShown = false;