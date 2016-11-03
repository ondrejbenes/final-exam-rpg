#pragma once

#define FAILED_TO_INITIALIZE 1
#define FAILED_TO_SHUT_OFF 2

#include "Module.h"

#include <SFML/Graphics/RenderWindow.hpp>

#include <map>
#include <memory>

/**
 * \brief Class responsible for Module init, running the game loop and Module destruction.
 */
class Engine
{
public:
	/**
	 * \brief Inits the Main Window and each of the Modules
	 * \return True, if all Modules were initialized successfully, false otherwise
	 */
	bool initialize();

	/**
	 * \brief Runs the main loop while the Game Module's isRunning flag returns true.
	 * Loop is based on the gamedesignepatterns. Modules are being updated until 
	 * we catch up with lag, then render is called.
	 * Lag is the elappsed time between updates.
	 * \return EXIT_SUCCESS if nothing unexpected happened, or an error code.
	 */
	int runGameLoop();

	/**
	 * \brief Closses the main window.
	 */
	bool shutOff();
private:
	static sf::Time MICRO_SEC_PER_UPDATE;

	std::shared_ptr<sf::RenderWindow> _mainWindow;
	std::map<ModuleType, std::unique_ptr<Module>> _modules;

	void initializeMainWindow();
	void handleWindowEvents() const;
};
