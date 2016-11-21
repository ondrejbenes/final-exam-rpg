#include "MainGame.h"
#include "Npc.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "Tilemap.h"
#include "Level.h"
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

#include <memory>
#include "Renderer.h"
#include <future>
#include "VectorUtilities.h"
#include "LevelLoader.h"

MainGame::MainGame() : 
_playerDied(false), 
_levelComplete(false),
_teleported(false),
_teleportClockRestarted(false),
_teleportFadedOut(false),
_donkeyTextShown(false)
{
	auto cursor = GetCursor();
	SetCursor(LoadCursor(nullptr, IDC_WAIT));
	EntityFactory factory;
	auto player = factory.create<Player>();

	auto entityManager = EntityManager::getInstance();
	entityManager->add(player);
	//player->setPosition(sf::Vector2f(4250, 2550)); //TEST
	player->setPosition(sf::Vector2f(600, 600));
	entityManager->setLocalPlayer(player);

	auto npc = factory.create<Npc>();
	//auto pos = sf::Vector2f(4120, 2310); //TESTs
	auto pos = sf::Vector2f(250, 230);
	npc->setPosition(pos);
	auto pc = npc->getComponent<AiComponent>();

	if (!Network::isMultiplayer() || Network::isServer())
		pc->ChangeState(new AiPatrol(pc, pos, 500));

	entityManager->add(npc);

	auto donkey = factory.createDonkey();
	donkey->setPosition(sf::Vector2f(900, 600));
	donkey->setName("donkey");
	entityManager->add(donkey);

	loadLevel("Resources/Images/tiles.png", "Resources/Levels/FinalExamMap_v2.csv");
	//loadLevel("Resources/Images/tilesTESTING.png", "Resources/Levels/FinalExamTileMapTESTING.csv");

	loadControls();
	SetCursor(cursor);

	Blackboard::getInstance()->leaveCallback(
		AUDIO,
		[](Module* target)
		{
			dynamic_cast<Audio*>(target)->playMusic(Audio::THEME_SONG);
		}
	);

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
	auto entityManager = EntityManager::getInstance();
	auto player = entityManager->getLocalPlayer();

	if (!_donkeyTextShown)
	{
		auto charaters = entityManager->getAllCharacters();
		sf::Vector2f donkeyPos;

		for (auto& x : charaters)
		{
			if (x->getName() == "donkey")
				donkeyPos = x->getPosition();
		}

		if (VectorUtilities::calculateDistance(player->getPosition(), donkeyPos) < 50)
		{
			_donkeyTextShown = true;
			showDonkeyText();
		}
	}

	if (player->getStats()->current_hitpoints == 0 && !_playerDied)
		handlePlayerDeath();

	// TODO assync?
	if (_playerDied && _playerDeathTimer.getElapsedTime() > sf::seconds(3))
	{
		GamePhaseManager::getInstance()->popPhase();
		return;
	}	

	if (EntityManager::getInstance()->getAllCharacters().size() == 1 && !_levelComplete)
		handleLevelComplete();

	// TODO assync?
	if (_levelComplete && _levelCompleteTimer.getElapsedTime() > sf::seconds(3))
	{
		GamePhaseManager::getInstance()->popPhase();
		return;
	}
	
	if(VectorUtilities::calculateDistance(player->getPosition(), sf::Vector2f(1100, 600)) < 100 && hasKeys())
	{
		if(!_teleportClockRestarted)
		{
			_teleportTimer.restart();
			_teleportClockRestarted = true;
		}
		teleportToArena();
	}

	handleInput();

	auto characters = EntityManager::getInstance()->getAllCharacters();
	for (auto it = characters.begin(); it != characters.end(); ++it)
	{
		if((*it)->getStats()->current_hitpoints != 0)
			(*it)->update();		
	}
}

void MainGame::render(std::shared_ptr<sf::RenderWindow> window)
{
	auto entityManager = EntityManager::getInstance();
	auto player = entityManager->getLocalPlayer();
	if(player == nullptr)
		return;
	auto playerPos = player->getPosition();
	auto boundary = QuadTreeBoundary(playerPos.x - 800, playerPos.x + 800, playerPos.y - 500, playerPos.y + 500);
	auto tiles = entityManager->getTilesInInterval(boundary);
	for (auto it = tiles.begin(); it != tiles.end(); ++it)
	{
		auto graphicsComponent = dynamic_cast<Entity*>(*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(window);
	}

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

void MainGame::showDonkeyText() 
{
	auto chatBoard = dynamic_cast<ChatBoard*>(_ui.getElementByName("chatBoard"));

	chatBoard->addMessage("Shiny Donkey", "Hi. I'm the Shiney Donkey");
	chatBoard->addMessage("Shiny Donkey", "You won't believe it, but I have a quest for you.");
	chatBoard->addMessage("Shiny Donkey", "You need to explore the island and look for keys.");
	chatBoard->addMessage("Shiny Donkey", "The keys will unlock the gate to the east of here.");
	chatBoard->addMessage("Shiny Donkey", "Then I need you to enter the tunnel.");
	chatBoard->addMessage("Shiny Donkey", "You will know what to do when you exit on the other side.");
}

void MainGame::teleportToArena() 
{
	auto player = EntityManager::getInstance()->getLocalPlayer();
	if (!_teleportFadedOut)
	{
		Blackboard::getInstance()->leaveCallback(
			RENDERER,
			[](Module* target)
		{
			dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You found a tunnel and gathered the courage to go forward...");
		}
		);
		_teleportFadedOut = true;
	}
	if(_teleportTimer.getElapsedTime().asSeconds() >= 2.9)
	{
		Blackboard::getInstance()->leaveCallback(
			RENDERER,
			[](Module* target)
			{
				dynamic_cast<Renderer*>(target)->fadeIn(sf::seconds(3), "The tunnel lead you to an arena filled with deamons!");
			}
		);
		EntityManager::getInstance()->move(player, arenaTeleportPosition);
		player->setPosition(arenaTeleportPosition);
	}
}

void MainGame::handlePlayerDeath() 
{
	Blackboard::getInstance()->leaveCallback(
		RENDERER,
		[](Module* target)
		{
			dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You died! Game over!");
		}
	);
	_playerDeathTimer.restart();
	_playerDied = true;
}

void MainGame::handleLevelComplete() 
{
	Blackboard::getInstance()->leaveCallback(
		RENDERER,
		[](Module* target)
		{
			dynamic_cast<Renderer*>(target)->fadeOut(sf::seconds(3), "You won!");
		}
	);
	_levelComplete = true;
	_levelCompleteTimer.restart();
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
				GamePhaseManager::getInstance()->popPhase();

				if(Network::isServer())
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

	if(!console->isVisible() && (!Network::isMultiplayer() || Network::isServer()))
		handleMovement();
}

void MainGame::handleMovement()
{
	auto localPlayer = EntityManager::getInstance()->getLocalPlayer();
	auto physicsComponent = localPlayer->getComponent<PhysicsComponent>();
	auto defaultVelocity = PhysicsComponent::defaultVelocity;
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

	physicsComponent->setVelocity(newVelocity);
}

bool MainGame::loadLevel(const std::string& textureFileName, const std::string& levelDefinitionFileName)
{
	_currentLevel = Level();
	_currentLevel.tilemap = new Tilemap();
	LevelLoader();
	if (!_currentLevel.tilemap->loadFromFile(textureFileName, levelDefinitionFileName))
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

bool MainGame::hasKeys() 
{
	auto player = EntityManager::getInstance()->getLocalPlayer();
	auto keysCount = 0;
	for(auto& item : player->getInventory())
	{
		if (item->getName() == "BronzeKey" || item->getName() == "SilverKey" || item->getName() == "GoldKey")
			keysCount++;
	}

	// TODO chagne to 3
	return keysCount >= 0;
}

Controls MainGame::CONTROLS = Controls();
const sf::Vector2f MainGame::arenaTeleportPosition = sf::Vector2f(200, 500);