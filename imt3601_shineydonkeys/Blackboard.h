#pragma once

#include "Module.h"

#include <vector>
#include <map>
#include <functional>
#include <queue>
#include <memory>

#include <SFML/Window/Event.hpp>

// Forward declaration
class Entity;


/**
 * \brief Blackboard is used for communicating between modules.
 * Singleton class.
 * For each module, there is a vector of callbacks.
 * The module will check the Blackboard in it's update method.
 * If there are any callbacks, it will inject itself as the callback's parameter.
 * Then the callback is called.
 * The Blackboard also has a queue of sf::WindowEvents.
 * Event are pushed there in the main loop (Engine class) 
 * and are usually popped in GamePhase.update().
 */
class Blackboard
{
public:
	/**
	 * \brief Gets the unique instance of this class.
	 * \return the unique instance
	 */
	static std::shared_ptr<Blackboard> getInstance();
	
	/**
	 * \brief Leave a callback for a module. Will be called in the next frame.
	 * \param moduleType Type of the module that is supposed to call the callback.
	 * \param callback The callback
	 */
	void leaveCallback(ModuleType moduleType, std::function<void(Module*)> callback);
	
	/**
	 * \brief Gets callbacks left for a module type. 
	 * The vector holding the callbacks is cleared.
	 * \param moduleType Usually the module from which this method is called
	 * \return Vector of callbacks
	 */
	std::vector<std::function<void(Module*)>> getCallbacks(ModuleType moduleType);

	/**
	 * \brief Push an event to the queue, usually to be picked up by a GamePhase
	 * \param e the Event
	 */
	void pushEvent(const sf::Event& e);

	/**
	 * \brief Polls a event from a queue.
	 * \param e Reference that the event will be assigned to
	 * \return true, if there was at least one event in the queue
	 */
	bool pollEvent(sf::Event& e);
protected:
	Blackboard() {}
private:
	static std::shared_ptr<Blackboard> instance;
	std::map<ModuleType, std::vector<std::function<void(Module*)>>> _allCallbacks;
	std::queue<sf::Event> _windowEvents;
};
