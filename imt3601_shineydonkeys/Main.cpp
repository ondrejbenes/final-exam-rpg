// TODO move someplace else
/*
Tiles 

1.Water			- blocks movement, but you can jump in it
2.Wet stone		- Slippery
3.Dry stone
4.Sand
5.Forest
6.Thick Forest	- You can hide, but only move back where you came from
7.Mud			- You will get stuck her 
8.Blocker		- You cant go here for some reason

Theese are the basic tiles, they can be replaced with overlays that look different but have same function as underlaying tilens.
*/

#include "Logger.h"
#include "Engine.h"

// TODO Check for memory leaks
// TODO Use only necessary includes in header files
// TODO Remove empty constructors and destructors

int main(int argc, char* argv[])
{
	auto exitCode = 0;
	LOG_D("Starting MAIN function");
		
	Engine engine;
	if (!engine.initialize())
	{
		LOG_E("Engine failed to initialize, application will exit.");
		return FAILED_TO_INITIALIZE;		
	}

	try
	{
		exitCode = engine.runGameLoop();
	}
	catch (std::exception& ex)
	{
		std::string message = "Exception propagated to top level: ";
		message.append(ex.what());
		LOG_E(message);
	}


	if (!engine.shutOff())
	{
		LOG_E("Engine failed to shut off");
		return FAILED_TO_SHUT_OFF;
	}

	LOG_I("Application exiting with code: " + exitCode);
	return exitCode;
}