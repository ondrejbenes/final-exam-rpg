#include "Renderer.h"
#include "EntityManager.h"
#include "GraphicsComponent.h"
#include "Tile.h"
#include "Console.h"
#include "GamePhaseManager.h"
#include "Blackboard.h"
#include "ResourceLoader.h"
#include "Logger.h"

Renderer::Renderer(std::shared_ptr<sf::RenderWindow> mainWindow) :
Module(RENDERER),
_mainWindow(mainWindow),
_zoom(1.0f)
{
	_windowWidth = _mainWindow->getSize().x;
	_windowHeight = _mainWindow->getSize().y;
}

bool Renderer::initialize()
{
	_overlay = sf::RectangleShape(_mainWindow->getView().getSize());
	return true;
}

void Renderer::update()
{
	auto callbacks = Blackboard::getInstance()->getCallbacks(moduleType);
	for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
		(*it)(this);
}

void Renderer::render()
{
	GamePhaseManager::getInstance()->getCurrentPhase()->render(_mainWindow);

	auto console = Console::getInstance();
	if (console->isVisible())
		console->draw(_mainWindow);

	auto player = EntityManager::getInstance()->getLocalPlayer();
	auto view = _mainWindow->getView();

	if (player != nullptr)
	{
		auto playerPos = player->getPosition();
		view.reset(sf::FloatRect(playerPos.x - _windowWidth / 2, playerPos.y - _windowHeight / 2, _windowWidth, _windowHeight));
	}
	else
	{
		view.reset(sf::FloatRect(0, 0, _windowWidth, _windowHeight));
	}

	if (_fadeIn)
		drawFadeIn();
	if (_fadeOut)
		drawFadeOut();

	// view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	view.zoom(_zoom);
	_mainWindow->setView(view);
}

void Renderer::resetWindowSize(const sf::Vector2u& newSize) 
{
	_windowWidth = newSize.x;
	_windowHeight = newSize.y;
}

void Renderer::fadeIn(const sf::Time& duration, const std::string& text)
{
	_fadeIn = true;
	_fadeOut = false;
	_fadeDuration = duration;
	_fadeClock.restart();
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	_fadeText = sf::Text(text, font);
}

void Renderer::fadeOut(const sf::Time& duration, const std::string& text)
{
	_fadeIn = false;
	_fadeOut = true;
	_fadeDuration = duration;
	_fadeClock.restart();
	auto& font = ResourceLoader::getInstance()->getMenuFont();
	_fadeText = sf::Text(text, font);
}

void Renderer::drawFadeIn() {
	auto elapsedTime = _fadeClock.getElapsedTime().asMilliseconds();
	if (elapsedTime >= _fadeDuration.asMilliseconds())
	{
		_fadeIn = false;
	}
	else
	{
		_overlay.setPosition(_mainWindow->mapPixelToCoords(sf::Vector2i(0, 0)));

		auto perc = elapsedTime / float(_fadeDuration.asMilliseconds());
		auto alpha = 255 - perc * 255;

		_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
		_mainWindow->draw(_overlay);
		auto x = _mainWindow->getView().getCenter().x - _fadeText.getGlobalBounds().width / 2;
		auto y = _mainWindow->getView().getCenter().y - _fadeText.getGlobalBounds().height / 2;
		_fadeText.setPosition(x, y);
		_mainWindow->draw(_fadeText);
	}
}

void Renderer::drawFadeOut() {
	auto elapsedTime = _fadeClock.getElapsedTime().asMilliseconds();

	if (elapsedTime >= _fadeDuration.asMilliseconds())
	{
		_fadeOut = false;
	}
	else
	{
		_overlay.setPosition(_mainWindow->mapPixelToCoords(sf::Vector2i(0, 0)));

		auto perc = std::min(1.0f, elapsedTime / float(_fadeDuration.asMilliseconds()));
		auto alpha = perc * 255;

		_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
		_mainWindow->draw(_overlay);
		auto x = _mainWindow->getView().getCenter().x - _fadeText.getGlobalBounds().width / 2;
		auto y = _mainWindow->getView().getCenter().y - _fadeText.getGlobalBounds().height / 2;
		_fadeText.setPosition(x, y);
		_mainWindow->draw(_fadeText);
	}
}