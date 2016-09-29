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
	LOG_I("Initializing Engine");
	engineState = INITIALIZING;

	// TODO Load from config file
	auto style = sf::Style::None;
	mainWindow = new sf::RenderWindow(sf::VideoMode(1280, 720), "Final Exam", style);
	mainWindow->setVerticalSyncEnabled(true);

	modules[RENDERER] = new Renderer(mainWindow);

	modules[GAME] = new Game();

	modules[AUDIO] = new Audio();

	modules[NETWORK] = new Network();

	auto success = true;
	for (auto it = modules.begin(); it != modules.end(); ++it)
		success = success && (*it).second->initialize();

	if(success)
		engineState = INITIALIZED;

	return success;
}

int Engine::runGameLoop()
{
	if (engineState != INITIALIZED)
		throw InvalidEngineStateException();

	LOG_I("Running game loop");

	engineState = RUNNING;

	while(mainWindow->isOpen())
	{
		// handle inputs

		for (auto it = modules.begin(); it != modules.end(); ++it)
			it->second->update();

		dynamic_cast<Renderer*>(modules[RENDERER])->render();

		mainWindow->display();
		mainWindow->clear();

		//sf::sleep(sf::milliseconds(1000/60));
	}


	return NORMAL_EXIT;
}

bool Engine::shutOff() 
{
	if (engineState != RUNNING)
		throw InvalidEngineStateException();
	engineState = SHUTTING_OFF;

	for (auto it = modules.begin(); it != modules.end(); ++it)
		delete it->second;

	delete mainWindow;

	engineState = SHUT_OFF;
	return true;
}

EngineState Engine::getEngineState()
{
	return engineState;
}
