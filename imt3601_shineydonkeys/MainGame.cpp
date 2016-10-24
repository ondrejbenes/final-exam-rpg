#include "MainGame.h"
#include "Npc.h"
#include "EntityManager.h"
#include "EntityFactory.h"
#include "Tilemap.h"
#include "Level.h"
#include "Game.h"
#include "Blackboard.h"
#include "GamePhaseManager.h"

MainGame::MainGame()
{
	EntityFactory factory;
	auto player = factory.create<Player>();

	auto entityManager = EntityManager::getInstance();
	entityManager->add(player);
	entityManager->setLocalPlayer(player);

	auto npc = factory.create<Npc>();
	npc->setPosition(sf::Vector2f(200, 150));
	entityManager->add(npc);

	loadLevel("level02.txt");
}

MainGame::~MainGame()
{
	EntityManager::getInstance()->clearCharacters();
}

void MainGame::update()
{
	handleInput();

	auto characters = EntityManager::getInstance()->getAllCharacters();
	for (auto it = characters.begin(); it != characters.end(); ++it)
		(*it)->update();
}

void MainGame::render(sf::RenderWindow* window)
{
	auto entityManager = EntityManager::getInstance();
	auto playerPos = entityManager->getLocalPlayer()->getPosition();
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

	GamePhase::render(window);
}

void MainGame::handleInput()
{
	auto blackboard = Blackboard::getInstance();
	sf::Event event;
	while (blackboard->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			if (event.key.code == sf::Keyboard::Escape)
			{
				GamePhaseManager::getInstance()->popPhase();
				return;
			}

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
			break;
		default:
			break;
		}
	}

	handleMovement();
}

void MainGame::handleMovement()
{
	auto localPlayer = EntityManager::getInstance()->getLocalPlayer();
	auto physicsComponent = localPlayer->getComponent<PhysicsComponent>();
	auto defaultVelocity = PhysicsComponent::defaultVelocity;

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
	{
		physicsComponent->setVelocity(sf::Vector2f(0, -1 * defaultVelocity.y));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
	{
		physicsComponent->setVelocity(sf::Vector2f(0, defaultVelocity.y));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		physicsComponent->setVelocity(sf::Vector2f(defaultVelocity.x, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		physicsComponent->setVelocity(sf::Vector2f(-1 * defaultVelocity.x, 0));
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		// TODO remove?
		//pc->move(Jump);
	}
	else
	{
		physicsComponent->setVelocity(sf::Vector2f(0, 0));
	}
	physicsComponent->move();
}

bool MainGame::loadLevel(std::string levelDefinition)
{
	_currentLevel = Level();
	_currentLevel.tilemap = new Tilemap();

	if (!_currentLevel.tilemap->loadFromFile(levelDefinition))
	{
		LOG_E("Failed to load level");
		return  false;
	}

	return true;;
}