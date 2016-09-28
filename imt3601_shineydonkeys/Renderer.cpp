#include "Renderer.h"
#include "EntityManager.h"
#include "Logger.h"
#include "GraphicsComponent.h"

Renderer::Renderer(sf::RenderWindow* mainWindow) : Module(RENDERER), mainWindow(mainWindow)
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
}
