#include "Engine.h"
#include "Logger.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include "Renderer.h"
#include "Game.h"
#include "Audio.h"
#include "Network.h"
#include "InvalidEngineStateException.h"
#include "ConfigFile.h"
#include "Blackboard.h"

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

	ConfigFile config;
	initializeMainWindow(config);

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

void Engine::initializeMainWindow(ConfigFile config)
{
	auto style = sf::Style::None;
	if (config.fullscreen)
		style = sf::Style::Fullscreen;
	mainWindow = new sf::RenderWindow(sf::VideoMode(config.resolution_width, config.resolution_height), "Final Exam", style);
	if (config.vSync)
		mainWindow->setVerticalSyncEnabled(true);
}

int Engine::runGameLoop()
{
	if (engineState != INITIALIZED)
		throw InvalidEngineStateException();

	LOG_I("Running game loop");

	engineState = RUNNING;

	while(mainWindow->isOpen())
	{
		sf::Event event;
		while (mainWindow->pollEvent(event))
		{
			switch (event.type)
			{
			case sf::Event::Closed:
				mainWindow->close();
				break;
			case sf::Event::KeyPressed:
				if (event.key.code == sf::Keyboard::Escape)
					mainWindow->close();
				else
					Blackboard::getInstance()->pushEvent(event);
				break;
			case sf::Event::TextEntered:
				Blackboard::getInstance()->pushEvent(event);
			default:
				break;
			}
		}

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
