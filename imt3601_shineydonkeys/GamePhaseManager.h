#pragma once

#include "GamePhase.h"

#include <stack>

/**
 * \brief A Singleton wrapper around a stack of GamePhases.
 * GamePhases include the Menus and the Main Game.
 */
class GamePhaseManager
{
public:
	~GamePhaseManager();
	/**
	* \brief Gets the unique instance of this class.
	* \return the unique instance
	*/
	static std::shared_ptr<GamePhaseManager> getInstance();

	/* Pushes the phase to the stack */
	void pushPhase(GamePhase* phase);

	/**
	 * \brief If there is a GamePhase in the stack, it will be popped and deleted.
	 * \return True, if there was a GamePhase to pop, false otherwise. 
	 */
	bool popPhase();

	/* Returns the phase that's on top of the stack */
	GamePhase* getCurrentPhase() const;
protected:
	GamePhaseManager(){}
private:
	static std::shared_ptr<GamePhaseManager> instance;
	std::stack<GamePhase*> _phases;
};
