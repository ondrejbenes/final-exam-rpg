#pragma once

#include <vector>
#include <map>
#include <functional>
#include <queue>
#include <SFML/Window/Event.hpp>

enum ModuleType;
class Entity;
class Module;

class Blackboard
{

public:
	virtual ~Blackboard();

	// TODO refactor singleton (http://stackoverflow.com/questions/1008019/c-singleton-design-pattern)
	static Blackboard* Blackboard::getInstance();

	void leaveCallback(ModuleType moduleType, std::function<void(Module*)> callback);
	std::vector<std::function<void(Module*)>> getCallbacks(ModuleType moduleType);

	void pushEvent(const sf::Event& e);
	bool pollEvent(sf::Event& e);
private:
	Blackboard();
	static Blackboard* instance;
	std::map<ModuleType, std::vector<std::function<void(Module*)>>> allCallbacks;
	std::queue<sf::Event> windowEvents;
};
