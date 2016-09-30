#pragma once
#include "Character.h"
#include "NpcActivity.h"

class EntityFactory;

class Npc : public Character
{
	friend EntityFactory;
public:
	virtual ~Npc();
	NpcActivity *m_NpcActivity;
protected:
	Npc();

};