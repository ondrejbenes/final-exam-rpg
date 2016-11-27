#pragma once
#include "Module.h"

#include <SFML/Graphics.hpp>
#include <memory>

class Renderer : public Module
{
public:
	explicit Renderer(std::shared_ptr<sf::RenderWindow> mainWindow);

	bool initialize() override;
	void update() override;

	void render();
	void setZoom(float zoom) { _zoom = zoom; }
	void resetWindowSize(const sf::Vector2u& newSize);

	void fadeIn(const sf::Time& duration, const std::string& text = "");
	void fadeOut(const sf::Time& duration, const std::string& text = "");
private:
	std::shared_ptr<sf::RenderWindow> _mainWindow;

	bool _fadeIn = false;
	bool _fadeOut = false;
	sf::Time _fadeDuration;
	sf::Clock _fadeClock;
	sf::Text _fadeText;
	sf::RectangleShape _overlay;

	unsigned int _windowWidth;
	unsigned int _windowHeight;

	float _zoom;

	void drawFadeIn();
	void drawFadeOut();
};
