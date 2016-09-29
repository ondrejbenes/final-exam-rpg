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

};
