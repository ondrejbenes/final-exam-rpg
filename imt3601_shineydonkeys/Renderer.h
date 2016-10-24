#pragma once
#include "Camera.h"
#include "Module.h"

#include <SFML/Graphics.hpp>

class Renderer : public Module
{
public:
	explicit Renderer(sf::RenderWindow* mainWindow);
	virtual ~Renderer();
	bool initialize() override;
	void update() override;

	void render();
	void setZoom(float zoom) { _camera.setZoom(zoom); }
private:
	sf::RenderWindow* _mainWindow;
	Camera _camera;
};
