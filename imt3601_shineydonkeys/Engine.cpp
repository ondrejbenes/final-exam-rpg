#include "Engine.h"
#include "Logger.h"
#include "Renderer.h"
#include "Game.h"
#include "Audio.h"
#include "Network.h"
#include "InvalidEngineStateException.h"
#include "ConfigFile.h"
#include "Blackboard.h"
#include "Console.h"

#include <SFML/Graphics/RenderWindow.hpp>

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

	auto game = dynamic_cast<Game*>(modules[GAME]);
	auto renderer = dynamic_cast<Renderer*>(modules[RENDERER]);

	sf::Clock gameLoopClock;
	sf::Time lag;

	while(game->isRunning())
	{
		auto elapsedTime = gameLoopClock.restart();
		lag += elapsedTime;

		handleWindowEvents();

		while(lag >= MICRO_SEC_PER_UPDATE)
		{
			for (auto it = modules.begin(); it != modules.end(); ++it)
				it->second->update();
			lag -= MICRO_SEC_PER_UPDATE;
		}

		renderer->render();

		mainWindow->display();
		mainWindow->clear();
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

	mainWindow->close();
	delete mainWindow;

	engineState = SHUT_OFF;
	return true;
}

EngineState Engine::getEngineState()
{
	return engineState;
}

void Engine::handleWindowEvents()
{
	sf::Event event;
	auto console = Console::getInstance();
	while (mainWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			mainWindow->close();
			break;
		case sf::Event::KeyPressed:
			/*if (event.key.code == sf::Keyboard::Escape)
				mainWindow->close();
			else */if (event.key.code == sf::Keyboard::Tilde)
				console->setVisible(!console->isVisible());
			else if (console->isVisible())
				console->handleEvent(event);
			else
				Blackboard::getInstance()->pushEvent(event);
			break;
		case sf::Event::TextEntered:
			if (console->isVisible())
				console->handleEvent(event);
			else
				Blackboard::getInstance()->pushEvent(event);
		default:
			Blackboard::getInstance()->pushEvent(event);
			break;
		}
	}
}

sf::Time Engine::MICRO_SEC_PER_UPDATE = sf::microseconds(1000000 / 60); // 60 FPS