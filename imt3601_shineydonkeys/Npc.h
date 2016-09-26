#pragma once
#include "Character.h"
#include "NpcActivity.h"

class Npc : public Character
{

public:
	Npc();
	virtual ~Npc();
	NpcActivity *m_NpcActivity;

};