#pragma once
#include "UI.h"
#include "Tilemap.h"

class Level
{

public:
	Level();
	virtual ~Level();
	UI *m_UI;
	Tilemap *m_Tilemap;

};