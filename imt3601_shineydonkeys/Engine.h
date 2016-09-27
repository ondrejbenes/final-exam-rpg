#pragma once
#include "Entity.h"
#include "Module.h"

enum EngineState
{
	SHUT_OFF,
	INITIALIZING,
	RUNNING,
	SHUTTING_OFF
};

class Engine
{
	std::vector<Entity*> entities;
	std::vector<Module*> modules;
	EngineState m_EngineState;
public:
	Engine();
	virtual ~Engine();

	bool initialize();
	bool shutOff();
	bool startGameLoop();

};