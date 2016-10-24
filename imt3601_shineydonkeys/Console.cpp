#include "Console.h"
#include "Logger.h"
#include "EntityManager.h"

#include <sstream>

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "Blackboard.h"
#include "Module.h"
#include "Renderer.h"

Console::Console() :
_regexMove("move\\(([0-9]+),([0-9]+),([0-9]+)\\)"),
_regexZoom("zoom\\(([0-9]+\\.?[0-9]*)\\)"),
visible(false)
{
	if (!font.loadFromFile("Resources/Fonts/sans.ttf"))
		LOG_E("Could not load font");

	history.push_back("This 'ere is a pretty console, innit?");

	_commands.push_back(std::make_pair(std::regex("help"), [&] { help(); }));
	_commands.push_back(std::make_pair(std::regex("list"), [&] { list(); }));
	_commands.push_back(std::make_pair(_regexMove, [&] { move(); }));
	_commands.push_back(std::make_pair(_regexZoom, [&] { zoom(); }));
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

bool Console::handleInput()
{
	history.push_back(input);

	for(auto it = _commands.begin(); it !=_commands.end(); ++it)
	{
		std::smatch match;
		if (regex_search(input, match, it->first))
		{
			it->second();
			return true;
		}
	}

	return false;
}

void Console::handleEvent(const sf::Event& event)
{
	switch (event.type)
	{
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::Return)
		{
			if (!handleInput())
				history.push_back("Failed to parse input");
			input = "";
		}
		break;
	case sf::Event::TextEntered:
		if (event.text.unicode == ';' || event.text.unicode == '\r' || event.text.unicode == '`')
			break;

		if (event.text.unicode == 8) // backspace
			input = input.substr(0, input.length() - 1);
		else
			input += event.text.unicode;
		break;
	default:
		LOG_D("Unknown event");
		break;
	}
}

void Console::draw(sf::RenderWindow* window) const
{
	auto linesCount = std::min(VISIBLE_MESSAGES_COUNT, static_cast<int>(history.size()));
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

	auto firstMsgIdx = std::max(int(history.size()) - VISIBLE_MESSAGES_COUNT, 0);

	for(auto i = 0; i < linesCount; i++)
	{
		auto text = sf::Text(history[firstMsgIdx + i], font, characterSize);
		text.setFillColor(sf::Color::White);
		text.setPosition(x + padding, y + padding + characterSize * i);
		window->draw(text);
	}

	auto text = sf::Text(inputMarker + input, font, characterSize);
	text.setFillColor(sf::Color::Magenta);
	text.setPosition(x + padding, y + padding + characterSize * linesCount);

	window->draw(text);
}

void Console::help()
{
	history.push_back("Allowed commands:");
	history.push_back("\t help: what you are reading now, dummy");
	history.push_back("\t list: lists all characters");
	history.push_back("\t move(uint id, uint x, uint y): moves entity with given id to given coordinates");
	history.push_back("\t zoom(ufloat Zoom): zoom camera (0-1 zooms in, >1 zooms out)");
}

void Console::list()
{
	history.push_back("Game entities:");
	auto entities = EntityManager::getInstance()->getAllCharacters();
	for (auto it = entities.begin(); it != entities.end(); ++it)
		history.push_back((*it)->toString());
}

void Console::move()
{
	std::smatch match;
	regex_search(input, match, _regexMove);

	auto id = stoi(match[1].str());
	auto x = stof(match[2].str());
	auto y = stof(match[3].str());
	sf::Vector2f newPos(x, y);

	auto entityManager = EntityManager::getInstance();
	auto entities = entityManager->getAllCharacters();
	for (auto it = entities.begin(); it != entities.end(); ++it)
		if ((*it)->id == id)
		{
			entityManager->move(*it, newPos);
			(*it)->setPosition(newPos);
		}
}

void Console::zoom()
{
	std::smatch match;
	regex_search(input, match, _regexZoom);

	auto zoomLevel = stof(match[1].str());

	Blackboard::getInstance()->leaveCallback(RENDERER, [zoomLevel](Module* target)
	{
		dynamic_cast<Renderer*>(target)->setZoom(zoomLevel);
	});
}

Console* Console::instance = nullptr;
int Console::VISIBLE_MESSAGES_COUNT = 10;
