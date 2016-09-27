#pragma once

#include <SFML/System/Vector2.hpp>

class Camera
{

public:
	Camera();
	virtual ~Camera();

private:
	sf::Vector2f bottomRight;
	sf::Vector2f topLeft;
	int zoomLevel;

};
