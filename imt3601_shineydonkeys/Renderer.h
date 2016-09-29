#pragma once
#include "Camera.h"
#include "Module.h"

#include <SFML/Graphics.hpp>

class Renderer : public Module
{
	sf::RenderWindow* mainWindow;
	Camera camera;
public:
	explicit Renderer(sf::RenderWindow* mainWindow);
	virtual ~Renderer();
	bool initialize() override;

	void render();

	sf::Texture TileTexture;
	sf::Sprite TileImage;

	int tile_x = 110;
	int tile_y = 75;
	int rows_x = 44;
	int rows_y = 44;
};
