#include "Renderer.h"
#include "EntityManager.h"
#include "GraphicsComponent.h"
#include "Tile.h"
#include "Console.h"
#include "GamePhaseManager.h"
#include "Blackboard.h"

Renderer::Renderer(std::shared_ptr<sf::RenderWindow> mainWindow) : 
Module(RENDERER), 
_mainWindow(mainWindow),
_camera(1.0f)
{

}

Renderer::~Renderer()
{

}

bool Renderer::initialize()
{

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

	if (player != nullptr)
	{
		auto playerPos = player->getPosition();
		_camera.reset(sf::FloatRect(playerPos.x - 980 / 2, playerPos.y - 660 / 2, 1280, 720));
	} else
	{
		_camera.reset(sf::FloatRect(0, 0, 1280, 720));
	}
	_camera.zoom(_camera.getZoom());
	_camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	_mainWindow->setView(_camera);
}
