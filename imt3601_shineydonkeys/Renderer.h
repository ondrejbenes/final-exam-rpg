#pragma once
#include "Camera.h"
#include "Module.h"

#include <SFML/Graphics.hpp>
#include <memory>

class Renderer : public Module
{
public:
	explicit Renderer(std::shared_ptr<sf::RenderWindow> mainWindow);
	virtual ~Renderer();
	bool initialize() override;
	void update() override;

	void render();
	void setZoom(float zoom) { _camera.setZoom(zoom); }
private:
	std::shared_ptr<sf::RenderWindow> _mainWindow;
	Camera _camera;
};
