#include "Game.h"
#include "EntityFactory.h"
#include "Logger.h"
#include "MainGame.h"
#include "EntityManager.h"
#include "GameStateSaver.h"
#include "Blackboard.h"
#include "GameStateLoader.h"

Game::Game() : Module(GAME)
{

}

Game::~Game()
{

}

bool Game::initialize()
{
	LOG_I("Initializing Game Module");
	bool success = true;

	EntityFactory factory;
	auto player = factory.create<Player>();

	auto entityManager = EntityManager::getInstance();
	entityManager->add(player);
	entityManager->setLocalPlayer(player);

	auto npc = factory.create<Npc>();
	npc->setPosition(sf::Vector2f(200, 150));
	entityManager->add(npc);
	
	// TODO change to mainMentu when it is
	gamePhase = new MainGame();

	// TODO Use ResourceLoader
	success = success && loadLevel("level02.txt");

	return success;
}

void Game::update()
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);

	auto characters = EntityManager::getInstance()->getAllCharacters();

	for (auto it = characters.begin(); it != characters.end(); ++it)
		(*it)->update();
}

void Game::quickSave()
{
	GameStateSaver saver;
	saver.quickSave();
}

void Game::quickLoad()
{
	GameStateLoader loader;
	loader.quickLoad();
}

bool Game::loadLevel(std::string levelDefinition)
{
	currentLevel = Level();
	currentLevel.tilemap = new Tilemap();

	if (!currentLevel.tilemap->loadFromFile(levelDefinition))
	{
		LOG_E("Failed to load level");
		return  false;
	}

	return true;;
}
