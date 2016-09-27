#pragma once
#include "Camera.h"
#include "Module.h"

#include <SFML/Graphics.hpp>

class Renderer : public Module
{
	sf::RenderWindow mainWindow;
public:
	Renderer();
	virtual ~Renderer();
	Camera *m_Camera;

	void render();

};
