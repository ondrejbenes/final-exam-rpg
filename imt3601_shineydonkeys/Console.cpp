#include "Console.h"
#include <SFML/Graphics/Text.hpp>
#include "Logger.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <sstream>
#include "EntityManager.h"
#include "StringUtilities.h"

Console::Console() :
visible(false)
{
	if (!font.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not load font");

	history.push_back("This 'ere is a pretty console, innit?");
}

Console::~Console()
{

}

Console* Console::getInstance()
{
	if (instance == nullptr)
		instance = new Console;
	return instance;
}

// TODO this should be refactored
bool Console::handleInput()
{
	history.push_back(input);
	// TODO create map<regex, callback>
	if(input == "help")
	{
		history.push_back("Allowed commands:");
		history.push_back("\t help: what you are reading now, dummy");
		history.push_back("\t list: lists all characters");
		history.push_back("\t move(id,x,y): moves entity with given id to given coordinates");

		input = "";
		return true;
	} 
	
	if (input == "list")
	{
		history.push_back("Game entities:");
		auto entities = EntityManager::gameEntities;
		for(auto it = entities.begin(); it != entities.end(); ++it)
			history.push_back((*it)->toString());
		history.push_back("Level entities:");
		entities = EntityManager::currentLevelEntities;
		for (auto it = entities.begin(); it != entities.end(); ++it)
			history.push_back((*it)->toString());

		input = "";
		return true;
	}

	if(StringUtilities::startsWith(input, std::string("move")))
	{
		auto params = StringUtilities::split(input, '(')[1];
		auto splitParams = StringUtilities::split(params, ',');
		splitParams[2].pop_back(); // get rid of ")"
		auto id = stoi(splitParams[0]);
		auto x = stof(splitParams[1]);
		auto y = stof(splitParams[2]);
		sf::Vector2f newPos(x, y);

		auto entities = EntityManager::gameEntities;
		for (auto it = entities.begin(); it != entities.end(); ++it)
			if ((*it)->id == id)
				(*it)->setPosition(newPos);

		entities = EntityManager::currentLevelEntities;
		for (auto it = entities.begin(); it != entities.end(); ++it)
			if ((*it)->id == id)
				(*it)->setPosition(newPos);

		input = "";
		return true;
	}

	input = "";
	return false;
}

void Console::draw(sf::RenderWindow* window) const
{
	auto linesCount = std::min(10, static_cast<int>(history.size()));
	auto view = window->getView();
	auto x = view.getCenter().x - view.getSize().x / 2.0f;
	auto y = view.getCenter().y - view.getSize().y / 2.0f;
	auto padding = 5;
	std::vector<sf::Text> lines(linesCount);

	auto bgWidth = view.getSize().x;
	auto bgHeight = (linesCount + 1) * characterSize + 2 * padding; // +1 for curr line
	sf::RectangleShape bg(sf::Vector2f(bgWidth, bgHeight));
	bg.setFillColor(sf::Color(0, 0, 0, 128));
	bg.setPosition(x, y);
	window->draw(bg);

	for(auto i = 0; i < linesCount; i++)
	{
		auto text = sf::Text(history[i], font, characterSize);
		text.setFillColor(sf::Color::White);
		text.setPosition(x + padding, y + padding + characterSize * i);
		window->draw(text);
	}

	auto text = sf::Text(inputMarker + input, font, characterSize);
	text.setFillColor(sf::Color::Magenta);
	text.setPosition(x + padding, y + padding + characterSize * linesCount);
	window->draw(text);
}

Console* Console::instance = nullptr;