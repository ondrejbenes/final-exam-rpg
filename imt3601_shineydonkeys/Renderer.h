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

	void fadeIn(const sf::Time& duration, const std::string& text = "");
	void fadeOut(const sf::Time& duration, const std::string& text = "");
private:
	std::shared_ptr<sf::RenderWindow> _mainWindow;
	Camera _camera;

	bool _fadeIn = false;
	bool _fadeOut = false;
	sf::Time _fadeDuration;
	sf::Clock _fadeClock;
	sf::Text _fadeText;
	sf::RectangleShape _overlay;

	void drawFadeIn();
	void drawFadeOut();
};
