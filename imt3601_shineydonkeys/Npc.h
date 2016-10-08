#pragma once
#include "Character.h"
#include "NpcActivity.h"

class EntityFactory;

class Npc : public Character
{
	friend EntityFactory;
public:
	NpcActivity *m_NpcActivity;

	virtual ~Npc();
protected:
	Npc();

};