#pragma once

#define NORMAL_EXIT 0
#define FAILED_TO_INITIALIZE 1
#define FAILED_TO_SHUT_OFF 2

#include "Module.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <map>

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
	std::map<ModuleType, Module*> modules;
	EngineState engineState;
public:
	Engine();
	virtual ~Engine();

	bool initialize();
	int runGameLoop();
	bool shutOff();
	EngineState getEngineState();
};