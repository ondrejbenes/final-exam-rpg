#pragma once

enum ModuleType
{
	AUDIO,
	GAME,
	NETWORK,
	RENDERER,
	SCHEDULER
};

class Module
{

public:
	explicit Module(ModuleType moduleType);

	virtual bool initialize() = 0;
	virtual void update() = 0;

	ModuleType getModuleType() const { return moduleType; }
protected:
	ModuleType moduleType;
};