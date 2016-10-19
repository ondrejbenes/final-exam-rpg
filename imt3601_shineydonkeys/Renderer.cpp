#include "Renderer.h"
#include "EntityManager.h"
#include "GraphicsComponent.h"
#include "Tile.h"
#include "Console.h"

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
	auto playerPos = EntityManager::localPlayer->getPosition();

	auto boundary = QuadTreeBoundary(playerPos.x - 800, playerPos.x + 800, playerPos.y - 500, playerPos.y + 500);
	auto tiles = EntityManager::tiles.getInInterval(&boundary);
	for (auto it = tiles.begin(); it != tiles.end(); ++it)
	{
		auto graphicsComponent = dynamic_cast<Entity*>(*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(mainWindow);
	}

	for (auto it = EntityManager::gameEntities.begin(); it != EntityManager::gameEntities.end(); ++it)
	{
		auto graphicsComponent = (*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(mainWindow);
	}

	for (auto it = EntityManager::currentLevelEntities.begin(); it != EntityManager::currentLevelEntities.end(); ++it)
	{
		auto graphicsComponent = (*it)->getComponent<GraphicsComponent>();
		if (graphicsComponent != nullptr)
			graphicsComponent->draw(mainWindow);
	}

	auto console = Console::getInstance();
	if (console->isVisible())
		console->draw(mainWindow);

	camera.reset(sf::FloatRect(playerPos.x - 980 / 2, playerPos.y - 660 / 2, 1280, 720));
	camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	// camera.zoom(2);
	mainWindow->setView(camera);
}
