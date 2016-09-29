#include "Renderer.h"
#include "EntityManager.h"
#include "Logger.h"
#include "GraphicsComponent.h"
#include "Tile.h"

Renderer::Renderer(sf::RenderWindow* mainWindow) : Module(RENDERER), mainWindow(mainWindow)
{

	if (!TileTexture.loadFromFile("Resources/Images/180tiles (3).png"))
		LOG_E("Error: could not load tile image");
	TileImage.setTexture(TileTexture);
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
		
	auto x = 0;	

	for (auto it = EntityManager::currentLevelTiles.begin(); it != EntityManager::currentLevelTiles.end(); ++it)
	{
		auto tile = static_cast<Tile*>(*it);
		TileImage.setTextureRect(sf::IntRect(tile->tileType / 7 * 180 + 1, tile->tileType % 7 * 180 + 1, 180 - 1, 180 - 1));
		mainWindow->draw(TileImage);
		//TileImage.setPosition(x%44*100+x/44%2*50,x/44*30-((framecount<180)*quake*(i%7*(180-framecount%180)))); //draws all tiles	
		TileImage.setPosition(x % rows_x * tile_x, x / rows_y * tile_y); //draws all tiles	
		x++; //counts tiles

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
}
