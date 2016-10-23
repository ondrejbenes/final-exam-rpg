#include "Renderer.h"
#include "EntityManager.h"
#include "GraphicsComponent.h"
#include "Tile.h"
#include "Console.h"
#include "GamePhaseManager.h"

Renderer::Renderer(sf::RenderWindow* mainWindow) : 
Module(RENDERER), 
mainWindow(mainWindow),
camera(0)
{

}

Renderer::~Renderer()
{

}

bool Renderer::initialize()
{

	return true;
}

void Renderer::render()
{
	/*
	for (auto it = EntityManager::currentLevelTiles.begin(); it != EntityManager::currentLevelTiles.end(); ++it)
	{
		auto graphicsComponent = (*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(mainWindow);
	}*/
	GamePhaseManager::getInstance()->getCurrentPhase()->render(mainWindow);


	auto console = Console::getInstance();
	if (console->isVisible())
		console->draw(mainWindow);


	auto player = EntityManager::getInstance()->getLocalPlayer();

	if (player != nullptr)
	{
		auto playerPos = player->getPosition();
		camera.reset(sf::FloatRect(playerPos.x - 980 / 2, playerPos.y - 660 / 2, 1280, 720));
	} else
	{
		camera.reset(sf::FloatRect(0, 0, 1280, 720));
	}
	camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	// camera.zoom(2);
	mainWindow->setView(camera);
}
