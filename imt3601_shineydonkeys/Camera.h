#pragma once

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/View.hpp>

class Camera : public sf::View
{

public:
	explicit Camera(int zoom_level);

	virtual ~Camera();

private:
	int zoomLevel;

};
