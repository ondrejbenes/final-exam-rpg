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
	for (auto it = EntityManager::currentLevelTiles.begin(); it != EntityManager::currentLevelTiles.end(); ++it)
	{
		auto graphicsComponent = (*it)->getComponent<GraphicsComponent>();
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

	auto position = EntityManager::localPlayer->getComponent<GraphicsComponent>()->getSprite().getPosition();
	camera.reset(sf::FloatRect(position.x - 980 / 2, position.y - 660 / 2, 1280, 720));
	camera.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));
	mainWindow->setView(camera);
}
