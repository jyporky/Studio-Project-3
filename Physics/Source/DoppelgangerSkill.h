#pragma once
#include "Skill.h"
#include "Player.h"
#include <string>
class Doppelganger : public Skill
{
public:
	Doppelganger();
	~Doppelganger();
	virtual std::string getDescription();
	virtual int getMoneyCost();
	virtual int getEnergyCost();
	virtual void UseSkill();
protected:
	Player* player;
};

