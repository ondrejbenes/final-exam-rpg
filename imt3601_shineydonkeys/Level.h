#pragma once
#include "UI.h"
#include "Tilemap.h"

class Level
{

public:
	Level();
	virtual ~Level();
	Tilemap* tilemap;
};