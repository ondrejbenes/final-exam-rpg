#include "Console.h"
#include "Logger.h"
#include "EntityManager.h"
#include "Blackboard.h"
#include "Module.h"
#include "Renderer.h"

#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Event.hpp>
#include "ResourceLoader.h"

Console::Console() :
_regexMove("move\\(([0-9]+),([0-9]+),([0-9]+)\\)"),
_regexZoom("zoom\\(([0-9]+\\.?[0-9]*)\\)")
{
	_history.push_back("Type help to get help (duh)");

	_commands.push_back(std::make_pair(std::regex("help"), [] { getInstance()->help(); }));
	_commands.push_back(std::make_pair(std::regex("list"), [] { getInstance()->list(); }));
	_commands.push_back(std::make_pair(_regexMove, [] { getInstance()->move(); }));
	_commands.push_back(std::make_pair(_regexZoom, [] { getInstance()->zoom(); }));
}

std::shared_ptr<Console> Console::getInstance()
{
	if (instance == nullptr)
		instance = std::make_shared<Console>(Console());
	return instance;
}

bool Console::handleInput()
{
	_history.push_back(_input);

	for(auto it = _commands.begin(); it !=_commands.end(); ++it)
	{
		std::smatch match;
		if (regex_search(_input, match, it->first))
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
				_history.push_back("Failed to parse input");
			_input = "";
		}
		break;
	case sf::Event::TextEntered:
		if (event.text.unicode == ';' || event.text.unicode == '\r' || event.text.unicode == '`')
			break;

		if (event.text.unicode == 8) // backspace
			_input = _input.substr(0, _input.length() - 1);
		else
			_input += event.text.unicode;
		break;
	default:
		LOG_D("Unknown event");
		break;
	}
}

void Console::draw(std::shared_ptr<sf::RenderWindow> window) const
{
	auto linesCount = std::min(size_t(_visibleMessagesCount), _history.size());
	auto view = window->getView();
	auto x = view.getCenter().x - view.getSize().x / 2.0f;
	auto y = view.getCenter().y - view.getSize().y / 2.0f;
	auto padding = 5;
	std::vector<sf::Text> lines(linesCount);

	auto bgWidth = view.getSize().x;
	auto bgHeight = (linesCount + 1) * _characterSize + 2 * padding; // +1 for curr line
	sf::RectangleShape bg(sf::Vector2f(bgWidth, bgHeight));
	bg.setFillColor(sf::Color(0, 0, 0, 128));
	bg.setPosition(x, y);
	window->draw(bg);

	auto firstMsgIdx = std::max(int(_history.size() - _visibleMessagesCount), 0);
	auto& font = ResourceLoader::getInstance()->getConsoleFont();

	for(auto i = 0; i < linesCount; i++)
	{
		auto text = sf::Text(_history[firstMsgIdx + i], font, _characterSize);
		text.setFillColor(sf::Color::White);
		text.setPosition(x + padding, y + padding + _characterSize * i);
		window->draw(text);
	}

	auto text = sf::Text(_inputMarker + _input, font, _characterSize);
	text.setFillColor(sf::Color::Magenta);
	text.setPosition(x + padding, y + padding + _characterSize * linesCount);

	window->draw(text);
}

void Console::help()
{
	_history.push_back("Allowed commands:");
	_history.push_back("\t help: what you are reading now, dummy");
	_history.push_back("\t list: lists all characters");
	_history.push_back("\t move(uint id, uint x, uint y): moves entity with given id to given coordinates");
	_history.push_back("\t zoom(ufloat Zoom): zoom camera (0-1 zooms in, >1 zooms out)");
}

void Console::list()
{
	_history.push_back("Game entities:");
	auto entities = EntityManager::getInstance()->getAllCharacters();
	for (auto it = entities.begin(); it != entities.end(); ++it)
		_history.push_back((*it)->toString());
}

void Console::move()
{
	std::smatch match;
	regex_search(_input, match, _regexMove);

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
	regex_search(_input, match, _regexZoom);

	auto zoomLevel = stof(match[1].str());

	Blackboard::getInstance()->leaveCallback(RENDERER, [zoomLevel](Module* target)
	{
		dynamic_cast<Renderer*>(target)->setZoom(zoomLevel);
	});
}

std::shared_ptr<Console> Console::instance = nullptr;