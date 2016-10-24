#pragma once

#include <SFML/Graphics/View.hpp>

class Camera : public sf::View
{

public:
	explicit Camera(float zoom_level);
	virtual ~Camera();

	// TODO Camera class probably unnacessary
	void setZoom(float zoomLevel) { _zoomLevel = zoomLevel; }
	float getZoom() { return _zoomLevel; }
private:
	float _zoomLevel;
};