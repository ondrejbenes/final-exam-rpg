#include "Game.h"
#include "Logger.h"
#include "GameStateSaver.h"
#include "Blackboard.h"
#include "GameStateLoader.h"
#include "GamePhaseManager.h"
#include "GamePhaseFactory.h"

Game::Game() : Module(GAME)
{

}

Game::~Game()
{
	LOG_I("Destroying Game Module.");
}

bool Game::initialize()
{
	LOG_I("Initializing Game Module");
	bool success = true;

	_running = true;

	GamePhaseFactory factory;

	GamePhaseManager::getInstance()->pushPhase(factory.createMainMenu());

	return success;
}

void Game::update()
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);

	GamePhaseManager::getInstance()->getCurrentPhase()->update();
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

void Game::stop()
{
	// TODO cleanup?

	_running = false;
}
