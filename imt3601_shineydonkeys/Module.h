#pragma once

enum ModuleType
{
	AUDIO,
	GAME,
	NETWORK,
	RENDERER
};

class Module
{

public:
	explicit Module(ModuleType moduleType);
	virtual ~Module();

	virtual bool initialize() = 0;
	virtual void update();

	ModuleType getModuleType() { return moduleType; }
protected:
	ModuleType moduleType;
};