#include "Logger.h"
#include "Engine.h"

int main(int argc, char* argv[])
{
	LOG_I("Starting main function");
		
	Engine engine;
	if (!engine.initialize())
	{
		LOG_E("Engine failed to initialize, application will exit.");
		return FAILED_TO_INITIALIZE;		
	}

	auto exitCode = engine.runGameLoop();

	if (!engine.shutOff())
	{
		LOG_E("Engine failed to shut off");
		return FAILED_TO_SHUT_OFF;
	}

	LOG_I("Application exiting with code: " + exitCode);
	return exitCode;
}