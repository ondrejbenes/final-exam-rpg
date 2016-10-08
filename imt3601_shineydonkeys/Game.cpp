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
	EntityManager::gameEntities.push_back(player);
	EntityManager::localPlayer = player;

	auto npc = factory.create<Npc>();
	npc->setPosition(sf::Vector2f(200, 150));
	EntityManager::gameEntities.push_back(npc);
	
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

	for (auto it = EntityManager::gameEntities.begin(); it != EntityManager::gameEntities.end(); ++it)
		(*it)->update();

	for (auto it = EntityManager::currentLevelEntities.begin(); it != EntityManager::currentLevelEntities.end(); ++it)
		(*it)->update();
}

void Game::saveGame()
{
	GameStateSaver saver;
	saver.saveGame();
}

void Game::loadGame()
{
	/*
	GameStateLoader loader;
	loader.loadGame();
	*/
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
