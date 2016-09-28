#pragma once

#define NORMAL_EXIT 0
#define FAILED_TO_INITIALIZE 1
#define FAILED_TO_SHUT_OFF 2

#include "Entity.h"
#include "Module.h"
#include <SFML/Graphics/RenderWindow.hpp>

enum EngineState
{
	SHUT_OFF,
	INITIALIZING,
	INITIALIZED,
	RUNNING,
	SHUTTING_OFF
};

class Engine
{
	sf::RenderWindow* mainWindow;
	std::vector<Entity*> entities;
	std::vector<Module*> modules;
	EngineState engineState;
public:
	Engine();
	virtual ~Engine();

	bool initialize();
	int startGameLoop();
	bool shutOff();
	EngineState getEngineState();
};