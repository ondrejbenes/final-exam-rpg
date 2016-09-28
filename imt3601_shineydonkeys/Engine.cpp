#include "Engine.h"
#include "Logger.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Renderer.h"
#include "Game.h"
#include "Audio.h"
#include "Network.h"
#include "InvalidEngineStateException.h"

Engine::Engine()
{

}

Engine::~Engine()
{

}

bool Engine::initialize()
{
	LOG_I("Initializing engine");
	engineState = INITIALIZING;

	// TODO Load from config file
	auto style = sf::Style::None;
	mainWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Final Exam", style);
	mainWindow->setVerticalSyncEnabled(true);

	modules.push_back(new Renderer(mainWindow));

	modules.push_back(new Game());

	modules.push_back(new Audio());

	modules.push_back(new Network());

	auto success = true;
	for (auto it = modules.begin(); it != modules.end(); ++it)
		success = success && (*it)->initialize();

	if(success)
		engineState = INITIALIZED;

	return success;
}

int Engine::startGameLoop()
{
	if (engineState != INITIALIZED)
		throw InvalidEngineStateException();

	engineState = RUNNING;

	// Game loop here


	return NORMAL_EXIT;
}

bool Engine::shutOff() 
{
	if (engineState != RUNNING)
		throw InvalidEngineStateException();
	engineState = SHUTTING_OFF;

	for (auto it = modules.begin(); it != modules.end(); ++it)
		delete *it;

	delete mainWindow;

	engineState = SHUT_OFF;
	return true;
}

EngineState Engine::getEngineState()
{
	return engineState;
}
