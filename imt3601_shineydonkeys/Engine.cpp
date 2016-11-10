#include "Engine.h"
#include "Logger.h"
#include "Renderer.h"
#include "Game.h"
#include "Audio.h"
#include "Network.h"
#include "ConfigIO.h"
#include "Blackboard.h"
#include "Console.h"

#include <SFML/Graphics/RenderWindow.hpp>

bool Engine::initialize()
{
	LOG_I("Initializing Engine");
	
	initializeMainWindow();

	_modules[RENDERER] = std::make_unique<Renderer>(_mainWindow);
	_modules[GAME] = std::make_unique<Game>();
	_modules[AUDIO] = std::make_unique<Audio>();
	_modules[NETWORK] = std::make_unique<Network>();

	auto success = true;
	for (auto it = _modules.begin(); it != _modules.end(); ++it)
		success = success && (*it).second->initialize();
	
	return success;
}

void Engine::initializeMainWindow()
{
	auto style = sf::Style::Default;
	auto desktop = sf::VideoMode::getDesktopMode();
	auto fullscreen = ConfigIO::readInt(L"graphics", L"fullscreen");
	auto resolutionWidth = ConfigIO::readInt(L"graphics", L"resolutionWidth", desktop.width);
	auto resolutionHeight = ConfigIO::readInt(L"graphics", L"resolutionHeight", desktop.height);
	auto vSync = ConfigIO::readInt(L"graphics", L"vSync");
	
	if (fullscreen)
		style = sf::Style::Fullscreen;
	_mainWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(resolutionWidth, resolutionHeight), "Final Exam", style);
	if (vSync)
		_mainWindow->setVerticalSyncEnabled(true);
}

int Engine::runGameLoop()
{
	LOG_I("Running game loop");

	auto game = dynamic_cast<Game*>(_modules[GAME].get());
	auto renderer = dynamic_cast<Renderer*>(_modules[RENDERER].get());

	sf::Clock gameLoopClock;
	sf::Time lag;

	while(game->isRunning())
	{
		auto elapsedTime = gameLoopClock.restart();
		lag += elapsedTime;

		handleWindowEvents();

		while(lag >= MICRO_SEC_PER_UPDATE)
		{
			for (auto it = _modules.begin(); it != _modules.end(); ++it)
				it->second->update();
			lag -= MICRO_SEC_PER_UPDATE;
		}

		renderer->render();

		_mainWindow->display();
		_mainWindow->clear();
	}

	return EXIT_SUCCESS;
}

bool Engine::shutOff()
{
	LOG_I("Shutting off the Engine");

	_mainWindow->close();

	return true;
}

void Engine::handleWindowEvents() const
{
	sf::Event event;
	while (_mainWindow->pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			dynamic_cast<Game*>(_modules.at(GAME).get())->stop();
			break;
		default:
			Blackboard::getInstance()->pushEvent(event);
			break;
		}
	}
}

sf::Time Engine::MICRO_SEC_PER_UPDATE = sf::microseconds(1000000 / 60); // 60 FPS