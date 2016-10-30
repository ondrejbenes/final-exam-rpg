#pragma once

#define NORMAL_EXIT 0
#define FAILED_TO_INITIALIZE 1
#define FAILED_TO_SHUT_OFF 2

#include "Module.h"
#include <SFML/Graphics/RenderWindow.hpp>
#include <map>
#include "ConfigIO.h"

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
public:
	Engine();
	virtual ~Engine();
	bool initialize();
	void initializeMainWindow(ConfigIO config);
	int runGameLoop();
	bool shutOff();
	EngineState getEngineState();
private:
	sf::RenderWindow* mainWindow;
	std::map<ModuleType, Module*> modules;
	EngineState engineState;
	static sf::Time MICRO_SEC_PER_UPDATE;

	void handleWindowEvents();
};