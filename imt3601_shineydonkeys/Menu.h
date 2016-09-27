#pragma once
#include "GamePhase.h"

class Menu : public GamePhase
{

public:
	Menu();
	virtual ~Menu();

	int onClick(int index);

};