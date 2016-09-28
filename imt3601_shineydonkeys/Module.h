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
	// TODO possibly unnecessary if used only in Engine game loop
	ModuleType moduleType;

	explicit Module(ModuleType moduleType);
	virtual ~Module();

	virtual bool initialize() = 0;
	virtual void update();

};